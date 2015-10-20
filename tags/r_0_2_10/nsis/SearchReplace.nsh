; search & replace line in file.
;-------------------------
; parameter description
; $9 is filename to search
; $8 is string to search
; $7 is string to replace
;------------------------
; How to call function:
;   Push "filename.txt"
;   Push "the string to search"
;   Push "the string to replace"
;   Call un.ReplaceLine
;-----------------------
Function un.ReplaceLine
 
 Pop $7
 Pop $8
 Pop $9

ClearErrors
FileOpen $0 $9 "r"                     ; open target file for reading
GetTempFileName $R0                            ; get new temp file name
FileOpen $1 $R0 "w"                            ; open temp file for writing
loop:
   FileRead $0 $2                              ; read line from target file
   IfErrors done                               ; check if end of file reached
   StrCmp $2 "$8$\r$\n" 0 +2      ; compare line with search string with CR/LF
   StrCpy $2 "$7$\r$\n"    ; change line
   StrCmp $2 "$8" 0 +2            ; compare line with search string without CR/LF (at the end of the file)
   StrCpy $2 "$7"          ; change line
   FileWrite $1 $2                             ; write changed or unchanged line to temp file
   Goto loop
 
done:
   FileClose $0                                ; close target file
   FileClose $1                                ; close temp file
   Delete $9                            ; delete target file
   CopyFiles /SILENT $R0 $9            ; copy temp file to target file
   Delete $R0                                  ; delete temp file
   
FunctionEnd

;---------


; search & delete line in file.
;-------------------------
; parameter description
; $9 is filename to search
; $8 is string to search and delete
; 
;------------------------
; How to call function:
;   
;   Push "filename.txt"
;   Push "the string to search and delete"
;   Call un.DeleteLine
;
;-----------------------
Function un.DeleteLine
 
 Pop $8
 Pop $9

ClearErrors
FileOpen $0 $9 "r"                     ; open target file for reading
GetTempFileName $R0                            ; get new temp file name
FileOpen $1 $R0 "w"                            ; open temp file for writing
loop:
   FileRead $0 $2                              ; read line from target file
   IfErrors done                               ; check if end of file reached
   StrCmp $2 "$8$\r$\n" loop 0    ; if search string (with CR/LF) is found, goto loop
   StrCmp $2 "$8" loop 0          ; if search string is found at the end of the file, goto loop
   FileWrite $1 $2                             ; write changed or unchanged line to temp file
   Goto loop
 
done:
   FileClose $0                                ; close target file
   FileClose $1                                ; close temp file
   Delete $9                            ; delete target file
   CopyFiles /SILENT $R0 $9            ; copy temp file to target file
   Delete $R0                                  ; delete temp file
   
FunctionEnd

;---------