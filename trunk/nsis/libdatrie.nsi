; NSIS installer
!include "MUI.nsh"
!include "Sections.nsh"
!include "logiclib.nsh"

;environment variable manipulation functions header 
!include "WriteEnvStr.nsh"
!include "WritePathStr.nsh"
!include "SearchReplace.nsh"
;-------------------------------
; change below variables.
;-------------------------------

!define PROGNAME "libdatrie"
!define VERSION "0.1.1"

;NSIS will add file(s) from this directory  
!define BINSRCDIR "${PROGNAME}"

!define EULAFILE "${BINSRCDIR}\doc\COPYING.txt"

;set env shell-script to store in /etc/profile.d (msys and cygwin)
!define ENV_SETFILE  "libdatrienv.sh"

;set BASEPATH variable name
!define BASEPATH "DATRIE_BASEPATH"
;--------------------------------

; The name of the installer
Name "${PROGNAME} v.${VERSION}"

; The file to write
OutFile "${PROGNAME}.v${VERSION}.exe"

ShowInstDetails show
ShowUninstDetails show

;--------------------------------
; additional variable for dependency checking. 
Var IndependentSectionState

;--------------------------------

;Interface Settings

    !define MUI_ABORTWARNING
    !define MUI_COMPONENTSPAGE_NODESC
;--------------------------------

;Pages
;Page EULA
;Page components
;Page directory
;Page instfiles

  !insertmacro MUI_PAGE_LICENSE "${EULAFILE}"
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES

;UninstPage components
;UninstPage uninstConfirm
;UninstPage instfiles
;  !insertmacro MUI_UNPAGE_COMPONENTS
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
;--------------------------------

;Languages
 
  !insertmacro MUI_LANGUAGE "English"

;--------------------------------

; Section install mainfiles

SectionGroup /e "${PROGNAME}-components"
;The stuff to install

 Section "-write information to registry"
   ;Write the installation path into the registry
  WriteRegStr HKLM "SOFTWARE\${PROGNAME}" "Install_Dir" "$INSTDIR"
  WriteRegStr HKLM "SOFTWARE\${PROGNAME}" "Version" "${VERSION}"
  WriteRegStr HKLM "SOFTWARE\${PROGNAME}" "BaseVar" "${BASEPATH}"
 SectionEnd
 
 Section "${PROGNAME}-runtime" runtimesec
  ;SectionIn RO
  ;Set output path to the installation directory.
  SetOutPath "$INSTDIR\bin"
  ;Put file there
  File "${BINSRCDIR}\bin\*.dll"
  
  SetOutPath "$INSTDIR\doc"
  File "${BINSRCDIR}\doc\*.*"
  ;Write component info.
  WriteRegStr HKLM "SOFTWARE\${PROGNAME}\components" "runtime" "1"
 SectionEnd
 
 Section /o "trietool" binsec
  ;SectionIn RO
  ;Set output path to the installation directory.
  SetOutPath "$INSTDIR\bin"
  ;Put file there
  File "${BINSRCDIR}\bin\*.exe"
  SetOutPath "$INSTDIR\share\man\man1"
  File "${BINSRCDIR}\share\man\man1\*.*" 
  ;Write component info.
  WriteRegStr HKLM "SOFTWARE\${PROGNAME}\components" "trietool" "1"
 SectionEnd


 Section /o "${PROGNAME}-devel" develsec
  ;SectionIn RO
  ;Set output path to the installation directory.
  SetOutPath $INSTDIR
  ;Put file there
  File /r "${BINSRCDIR}\include"
  
  SetOutPath "$INSTDIR\lib"
  File "${BINSRCDIR}\lib\*.*"
  
  SetOutPath "$INSTDIR\lib\pkgconfig"
  File "${BINSRCDIR}\lib\pkgconfig\*.*"
 
   ;Write component info.
  WriteRegStr HKLM "SOFTWARE\${PROGNAME}\components" "devel" "1"
 SectionEnd
 
 Section /o "${PROGNAME}-doc" docsec
  ;SectionIn RO
  ;Set output path to the installation directory.
  SetOutPath "$INSTDIR\share"
  ;Put file there
  File /r "${BINSRCDIR}\share\doc"
   ;Write component info.
  WriteRegStr HKLM "SOFTWARE\${PROGNAME}\components" "doc" "1"    
 SectionEnd
 
 Section "-Write Uninstaller"
  ;Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PROGNAME}" "DisplayName" "${PROGNAME} v.${VERSION}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PROGNAME}" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PROGNAME}" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PROGNAME}" "NoRepair" 1
  ;create uninstaller
  SetOutPath $INSTDIR
  WriteUninstaller "$INSTDIR\uninstall.exe"
 SectionEnd
 
 Section "Start Menu Shortcuts"
  SetShellVarContext current
  CreateDirectory "$STARTMENU\Programs\${PROGNAME}"
  ; Create link to docs if runtime section is selected
  ${If} ${SectionIsSelected} ${runtimesec}
    CreateShortCut "$STARTMENU\Programs\${PROGNAME}\AUTHORS.lnk" "$INSTDIR\doc\AUTHORS.txt"
    CreateShortCut "$STARTMENU\Programs\${PROGNAME}\COPYING.lnk" "$INSTDIR\doc\COPYING.txt"
    CreateShortCut "$STARTMENU\Programs\${PROGNAME}\INSTALL.lnk" "$INSTDIR\doc\INSTALL.txt"
    CreateShortCut "$STARTMENU\Programs\${PROGNAME}\NEWS.lnk" "$INSTDIR\doc\NEWS.txt"
    CreateShortCut "$STARTMENU\Programs\${PROGNAME}\README.lnk" "$INSTDIR\doc\README.txt" 
    CreateShortCut "$STARTMENU\Programs\${PROGNAME}\Changelog.lnk" "$INSTDIR\doc\Changelog.txt"
  ${EndIf} 
  CreateShortCut "$STARTMENU\Programs\${PROGNAME}\Uninstall.lnk" "$INSTDIR\uninstall.exe"  
   ;Write component info.
  WriteRegStr HKLM "SOFTWARE\${PROGNAME}\components" "startmenu" "1"       
 SectionEnd

SectionGroupEnd

;-------------------------------------------------------------
; Section set environment

SectionGroup /e "Set environment for toolchains"
  
 Section "-set base path"  
  ReadRegStr $0 HKLM "SOFTWARE\${PROGNAME}" "BaseVar"
  ReadRegStr $1 HKLM "SOFTWARE\${PROGNAME}" "Install_Dir"
  Push $0
  Push '"$1"'
  Call WriteEnvStr
 SectionEnd
 
 Section "-set bin path" 
  ; check if runtime section is selected.
  ${If} ${SectionIsSelected} ${runtimesec}
   ReadRegStr $0 HKLM "SOFTWARE\${PROGNAME}" "Install_Dir" 
   ReadRegStr $1 HKLM "SOFTWARE\Microsoft\Windows NT\CurrentVersion" CurrentVersion
   StrCmp $1 "" noNT isNT                                                          
    noNT:                                                                          
     Push "PATH"                                          
     Push '"$0\bin";%PATH%'                                  
     Call WriteEnvStr                                                                                                                          
     goto done                                                                     
    isNT:                                                                          
     Push "PATH"                                          
     Push '"$0\bin"'                                  
     Call AddToEnvVar
     goto done
    done:
  ${EndIf}
 SectionEnd

 Section "MSYS/mingw"
  ;where is MSYS/mingw
  ReadRegStr $0 HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\MSYS-1.0_is1" "Inno Setup: App Path"
  StrCmp $0 "" nomsys ismsys
   nomsys:
    goto done
   ismsys:
    SetOutPath "$0\etc\profile.d"
    File /oname=${ENV_SETFILE} "${BINSRCDIR}\contrib\${ENV_SETFILE}"  
    WriteRegStr HKLM "SOFTWARE\${PROGNAME}" "msys-env" "$0\etc\profile.d\${ENV_SETFILE}"
    goto done
   done:
 SectionEnd

 Section "Cygwin"
  ;where is Cygwin
  ReadRegStr $0 HKLM "Software\Cygnus Solutions\Cygwin\mounts v2\/" "native"
  StrCmp $0 "" nocygwin iscygwin
   nocygwin:
    goto done
   iscygwin:
    SetOutPath "$0\etc\profile.d"
    File /oname=${ENV_SETFILE} "${BINSRCDIR}\contrib\${ENV_SETFILE}"
    WriteRegStr HKLM "SOFTWARE\${PROGNAME}" "cygwin-env" "$0\etc\profile.d\${ENV_SETFILE}"
    goto done
   done:
 SectionEnd

 Section "MSVC"
  ReadRegStr $0 HKLM "SOFTWARE\${PROGNAME}" "Install_Dir" 
  ReadRegStr $1 HKLM "SOFTWARE\Microsoft\Windows NT\CurrentVersion" CurrentVersion
  StrCmp $1 "" noNT0 isNT0                                                          
   noNT0:                                                                          
    Push "INCLUDE"                                          
    Push '"$0\include";%INCLUDE%'                                  
    Call WriteEnvStr                                                                                                                          
    goto inc_done                                                                     
   isNT0:                                                                          
    Push "INCLUDE"                                          
    Push '"$0\include"'                                  
    Call AddToEnvVar
    goto inc_done
   inc_done:

  ReadRegStr $0 HKLM "SOFTWARE\${PROGNAME}" "Install_Dir" 
  ReadRegStr $1 HKLM "SOFTWARE\Microsoft\Windows NT\CurrentVersion" CurrentVersion
  StrCmp $1 "" noNT1 isNT1                                                          
   noNT1:                                                                          
    Push "LIB"
    Push '"$0\lib";%LIB%'
    Call WriteEnvStr
    goto lib_done 
   isNT1:
    Push "LIB"
    Push '"$0\lib"'
    Call AddToEnvVar
    goto lib_done 
   lib_done:
  
  SectionEnd

SectionGroupEnd

;--------------------------------
; Uninstall Sections.
Section "-un.Uninstall"
 ;Get installation folder from registry if available
 ; Remove files and uninstaller
 ReadRegStr $0 HKLM "SOFTWARE\${PROGNAME}" "Install_Dir"
 RMDir /r "$0"
 
 ; Remove environment Variables
 ; Remove BASEPATH
 ReadRegStr $0 HKLM "SOFTWARE\${PROGNAME}" "BaseVar"
 Push $0
 Call un.DeleteEnvStr

 ; Remove bin path
 ReadRegStr $0 HKLM "SOFTWARE\${PROGNAME}" "Install_Dir" 
 ReadRegStr $1 HKLM "SOFTWARE\Microsoft\Windows NT\CurrentVersion" CurrentVersion
  StrCmp $1 "" noNT0 isNT0                                                          
   noNT0:                                                                          
    StrCpy $2 $WINDIR 2
    Push "$2\autoexec.bat"
    Push 'SET PATH="$0\bin";%PATH%'
    Call un.DeleteLine
    goto bin_done                                                                     
   isNT0:                                                                          
    Push "PATH"                                          
    Push '"$0\bin"'                                  
    Call un.RemoveFromEnvVar                         
    goto bin_done
   bin_done:
 
 ; Remove include path
 ReadRegStr $0 HKLM "SOFTWARE\${PROGNAME}" "Install_Dir" 
 ReadRegStr $1 HKLM "SOFTWARE\Microsoft\Windows NT\CurrentVersion" CurrentVersion
  StrCmp $1 "" noNT1 isNT1                                                          
   noNT1:                                                                          
    StrCpy $2 $WINDIR 2
    Push "$2\autoexec.bat"
    Push 'SET INCLUDE="$0\include";%INCLUDE%'
    Call un.DeleteLine
    goto inc_done                                                                     
   isNT1:                                                                          
    Push "INCLUDE"
    Push '"$0\include"'
    Call un.RemoveFromEnvVar
    goto inc_done
   inc_done:

 ; Remove lib path
 ReadRegStr $0 HKLM "SOFTWARE\${PROGNAME}" "Install_Dir" 
 ReadRegStr $1 HKLM "SOFTWARE\Microsoft\Windows NT\CurrentVersion" CurrentVersion
  StrCmp $1 "" noNT2 isNT2                                                          
   noNT2:                                                                          
    StrCpy $2 $WINDIR 2
    Push "$2\autoexec.bat"
    Push 'SET LIB="$0\lib";%LIB%'
    Call un.DeleteLine
    goto lib_done                                                                     
   isNT2:                                                                          
    Push "LIB"
    Push '"$0\lib"'
    Call un.RemoveFromEnvVar
    goto lib_done
   lib_done:
 
 ; Remove shell-script in /etc/profile.d (msys/cygwin)
 ReadRegStr $0 HKLM "SOFTWARE\${PROGNAME}" "msys-env"
 Delete $0
 ReadRegStr $0 HKLM "SOFTWARE\${PROGNAME}" "cygwin-env"
 Delete $0
  
 ; Remove shortcuts, if any
 SetShellVarContext current
 RMDir /r "$STARTMENU\Programs\${PROGNAME}"
  
 ; Remove registry keys
 DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PROGNAME}"
 DeleteRegKey HKLM "SOFTWARE\${PROGNAME}"
SectionEnd

;---------------------------------------

Function .onInit
    ; The default installation directory is "systemdrive:\(program's name)"
    ; for example :  "C:\${PROGNAME}, D:$\{PROGNAME}"
    StrCpy $R9 $WINDIR 2
    StrCpy $INSTDIR "$R9\${PROGNAME}"
    
    ; Dependency Checking's variable setting.
    ;This is first time setting.
    SectionGetFlags ${runtimesec} $R0
    IntOp $R0 $R0 & ${SF_SELECTED}
    StrCpy $IndependentSectionState $R0

FunctionEnd


;-------------------------------------------------------------
; use callback function .onSelChange for dependency checking.
;-------------------------------------------------------------
Function .onSelChange

;==================================
; start checking dependcy routine.
;==================================
Push $R0
Push $R1
 
  # Check if runtimesec was just selected then select binsec and develsec.
  SectionGetFlags ${runtimesec} $R0
  IntOp $R0 $R0 & ${SF_SELECTED}
  StrCmp $R0 $IndependentSectionState +3
    StrCpy $IndependentSectionState $R0
  Goto UnselectDependentSections
    StrCpy $IndependentSectionState $R0
 
  Goto CheckDependentSections
 
  # Select runtimesec if binsec or develsec was selected.
  SelectIndependentSection:
 
    SectionGetFlags ${runtimesec} $R0
    IntOp $R1 $R0 & ${SF_SELECTED}
    StrCmp $R1 ${SF_SELECTED} +3
 
    IntOp $R0 $R0 | ${SF_SELECTED}
    SectionSetFlags ${runtimesec} $R0
 
    StrCpy $IndependentSectionState ${SF_SELECTED}
 
  Goto End
 
  # Were binsec or develsec just unselected?
  CheckDependentSections:
 
  SectionGetFlags ${binsec} $R0
  IntOp $R0 $R0 & ${SF_SELECTED}
  StrCmp $R0 ${SF_SELECTED} SelectIndependentSection
 
  SectionGetFlags ${develsec} $R0
  IntOp $R0 $R0 & ${SF_SELECTED}
  StrCmp $R0 ${SF_SELECTED} SelectIndependentSection
 
  Goto End
 
  # Unselect binsec and develsec if runtimesec was unselected.
  UnselectDependentSections:
 
    SectionGetFlags ${binsec} $R0
    IntOp $R1 $R0 & ${SF_SELECTED}
    StrCmp $R1 ${SF_SELECTED} 0 +3
 
    IntOp $R0 $R0 ^ ${SF_SELECTED}
    SectionSetFlags ${binsec} $R0
 
    SectionGetFlags ${develsec} $R0
    IntOp $R1 $R0 & ${SF_SELECTED}
    StrCmp $R1 ${SF_SELECTED} 0 +3
 
    IntOp $R0 $R0 ^ ${SF_SELECTED}
    SectionSetFlags ${develsec} $R0
 
  End:
 
Pop $R1
Pop $R0
;==================================
; End of checking dependcy routine.
;==================================

FunctionEnd
;-----------------------------
