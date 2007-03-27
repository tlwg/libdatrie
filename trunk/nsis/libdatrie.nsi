; NSIS installer
!include "MUI.nsh"

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

!define EULAFILE "${BINSRCDIR}/doc/COPYING"

;set env shell-script to store in /etc/profile.d (msys and cygwin)
!define ENV_SETFILE  "libdatrienv.sh"

;set BASEPATH variable name
!define BASEPATH "DATRIE_BASEPATH"
;--------------------------------

; The name of the installer
Name "${PROGNAME} v.${VERSION}"

; The file to write
OutFile "${PROGNAME}.v${VERSION}.exe"

; The default installation directory
InstallDir "$PROGRAMFILES\${PROGNAME}"

ShowInstDetails show
ShowUninstDetails show

;--------------------------------

;Interface Settings

  !define MUI_ABORTWARNING

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

;UninstPage uninstConfirm
;UninstPage instfiles

  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
;--------------------------------

;Languages
 
  !insertmacro MUI_LANGUAGE "English"

;--------------------------------

; Section install mainfiles

SectionGroup /e "${PROGNAME}"
;The stuff to install

 Section "${PROGNAME} v.${VERSION}(required)"
  SectionIn RO
  ;Set output path to the installation directory.
  SetOutPath $INSTDIR
  
  ;Put file there
  File /r "${BINSRCDIR}\*.*"
  
  ;Write the installation path into the registry
  WriteRegStr HKLM "SOFTWARE\${PROGNAME}" "Install_Dir" "$INSTDIR"
  WriteRegStr HKLM "SOFTWARE\${PROGNAME}" "Version" "${VERSION}"
  WriteRegStr HKLM "SOFTWARE\${PROGNAME}" "BaseVar" "${BASEPATH}"
  ;Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PROGNAME}" "DisplayName" "${PROGNAME} v.${VERSION}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PROGNAME}" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PROGNAME}" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PROGNAME}" "NoRepair" 1
  ;create uninstaller
  WriteUninstaller "$INSTDIR\uninstall.exe"
 
 SectionEnd

 Section "Start Menu Shortcuts"

  SetShellVarContext current
  CreateDirectory "$STARTMENU\Programs\${PROGNAME}"
  CreateShortCut "$STARTMENU\Programs\${PROGNAME}\AUTHORS.lnk" "$INSTDIR\doc\AUTHORS.txt"
  CreateShortCut "$STARTMENU\Programs\${PROGNAME}\COPYING.lnk" "$INSTDIR\doc\COPYING.txt"
  CreateShortCut "$STARTMENU\Programs\${PROGNAME}\INSTALL.lnk" "$INSTDIR\doc\INSTALL.txt"
  CreateShortCut "$STARTMENU\Programs\${PROGNAME}\NEWS.lnk" "$INSTDIR\doc\NEWS.txt"
  CreateShortCut "$STARTMENU\Programs\${PROGNAME}\README.lnk" "$INSTDIR\doc\README.txt" 
  CreateShortCut "$STARTMENU\Programs\${PROGNAME}\Changelog.lnk" "$INSTDIR\doc\Changelog.txt"
  CreateShortCut "$STARTMENU\Programs\${PROGNAME}\Uninstall.lnk" "$INSTDIR\uninstall.exe"  
 SectionEnd

SectionGroupEnd

;-------------------------------------------------------------
; Section set environment

SectionGroup /e "Set evironment for toolchains"
  
 Section "-set base path"  
  ReadRegStr $0 HKLM "SOFTWARE\${PROGNAME}" "BaseVar"
  ReadRegStr $1 HKLM "SOFTWARE\${PROGNAME}" "Install_Dir"
  Push $0
  Push '"$1"'
  Call WriteEnvStr
 SectionEnd
 
 Section "-set bin path" 
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

; Uninstaller

Section "Uninstall"
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
