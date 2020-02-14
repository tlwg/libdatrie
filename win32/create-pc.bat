@echo off
rem Simple .bat script for creating the NMake Makefile snippets.

if "%1" == "" goto :error_no_dest
if "%2" == "" goto :error_no_ver
if "%3" == "" goto :error_dst

set pfx=%~dnp3
echo prefix=%pfx:\=/%>%1
echo exec_prefix=^$^{prefix^}>>%1
echo libdir=^$^{prefix^}/lib>>%1
echo includedir=^$^{prefix^}/include>>%1
echo.>>%1
echo Name: datrie>>%1
echo Description: Double-array trie library>>%1
echo Version: %2>>%1
echo Libs: -L^$^{libdir^} -ldatrie>>%1
echo Cflags: -I^$^{includedir^}>>%1
goto done

:error_no_dest
echo No output file specified
goto done

:error_no_ver
echo No version specified
goto done

:error_dst
echo A prefix path must be specified
goto done

:done