Instructions for building libdatrie on Visual Studio
====================================================
Building the libdatrie on Windows is now supported using Visual Studio.

You will need the following items to build libdatrie using Visual Studio:
-libiconv (including the libcharset headers and libraries) if building trietool.exe

Invoke the build by issuing the command:
nmake /f Makefile.vc CFG=[release|debug] [PREFIX=...] <option1=1 option2=1 ...>
where:

CFG: Required.  Choose from a release or debug build.  Note that
     all builds generate a .pdb file for each .dll and .exe built--this refers
     to the C/C++ runtime that the build uses.

PREFIX: Optional.  Base directory of where the third-party headers, libraries
        and needed tools can be found, i.e. headers in $(PREFIX)\include,
        libraries in $(PREFIX)\lib and tools in $(PREFIX)\bin.  If not
        specified, $(PREFIX) is set as $(srcroot)\..\vs$(X)\$(platform), where
        $(platform) is win32 for 32-bit builds or x64 for 64-bit builds, and
        $(X) is the short version of the Visual Studio used, as follows:
        2008: 9
        2010: 10
        2012: 11
        2013: 12
        2015, 2017, 2019: 14

Explanation of options, set by <option>=1:
------------------------------------------
LIB_ONLY: Disables building trietool.exe, which eliminates the need for libiconv.

An 'install' target is provided to copy the built items and public headers to appropriate
locations under $(PREFIX), a 'clean' target is provided to remove all the items that are
created during the build, and a 'tests' target is provided to build the test programs.
