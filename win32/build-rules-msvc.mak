# NMake Makefile portion for compilation rules
# Items in here should not need to be edited unless
# one is maintaining the NMake build files.  The format
# of NMake Makefiles here are different from the GNU
# Makefiles.  Please see the comments about these formats.

# Inference rules for compiling the .obj files.
# Used for libs and programs with more than a single source file.
# Format is as follows
# (all dirs must have a trailing '\'):
#
# {$(srcdir)}.$(srcext){$(destdir)}.obj::
# 	$(CC)|$(CXX) $(cflags) /Fo$(destdir) /c @<<
# $<
# <<
{..\datrie\}.c{vs$(PDBVER)\$(CFG)\$(PLAT)\datrie\}.obj::
	$(CC) $(CFLAGS) $(LIBDATRIE_BASE_CFLAGS) /Fovs$(PDBVER)\$(CFG)\$(PLAT)\datrie\ /Fdvs$(PDBVER)\$(CFG)\$(PLAT)\datrie\ /c @<<
$<
<<

{..\tools\}.c{vs$(PDBVER)\$(CFG)\$(PLAT)\trietool\}.obj::
	$(CC) $(CFLAGS) $(TRIETOOL_CFLAGS) /Fovs$(PDBVER)\$(CFG)\$(PLAT)\trietool\ /Fdvs$(PDBVER)\$(CFG)\$(PLAT)\trietool\ /c @<<
$<
<<

{..\tests\}.c{vs$(PDBVER)\$(CFG)\$(PLAT)\datrie-tests\}.obj::
	@if not exist vs$(PDBVER)\$(CFG)\$(PLAT)\datrie-tests\ $(MAKE) /f Makefile.vc CFG=$(CFG) vs$(PDBVER)\$(CFG)\$(PLAT)\datrie-tests
	$(CC) $(CFLAGS) $(LIBDATRIE_BASE_CFLAGS) /Fovs$(PDBVER)\$(CFG)\$(PLAT)\datrie-tests\ /Fdvs$(PDBVER)\$(CFG)\$(PLAT)\datrie-tests\ /c @<<
$<
<<

# Rules for building .lib files
vs$(PDBVER)\$(CFG)\$(PLAT)\datrie.lib: vs$(PDBVER)\$(CFG)\$(PLAT)\datrie.dll

# Rules for linking DLLs
# Format is as follows (the mt command is needed for MSVC 2005/2008 builds):
# $(dll_name_with_path): $(dependent_libs_files_objects_and_items)
#	link /DLL [$(linker_flags)] [$(dependent_libs)] [/def:$(def_file_if_used)] [/implib:$(lib_name_if_needed)] -out:$@ @<<
# $(dependent_objects)
# <<
# 	@-if exist $@.manifest mt /manifest $@.manifest /outputresource:$@;2
vs$(PDBVER)\$(CFG)\$(PLAT)\datrie.dll:		\
vs$(PDBVER)\$(CFG)\$(PLAT)\datrie\datrie.def	\
vs$(PDBVER)\$(CFG)\$(PLAT)\datrie-0.2.pc	\
$(libdatrie_dll_OBJS)
	link /DLL $(LDFLAGS) /implib:$(@R).lib /def:vs$(PDBVER)\$(CFG)\$(PLAT)\datrie\datrie.def	\
	-out:$@ @<<
$(libdatrie_dll_OBJS)
<<
	@-if exist $@.manifest mt /manifest $@.manifest /outputresource:$@;2

# Rules for linking Executables
# Format is as follows (the mt command is needed for MSVC 2005/2008 builds):
# $(dll_name_with_path): $(dependent_libs_files_objects_and_items)
#	link [$(linker_flags)] [$(dependent_libs)] -out:$@ @<<
# $(dependent_objects)
# <<
# 	@-if exist $@.manifest mt /manifest $@.manifest /outputresource:$@;1

vs$(PDBVER)\$(CFG)\$(PLAT)\trietool.exe: vs$(PDBVER)\$(CFG)\$(PLAT)\datrie.lib vs$(PDBVER)\$(CFG)\$(PLAT)\trietool\config.h $(trietool_OBJS)
	link $(LDFLAGS) vs$(PDBVER)\$(CFG)\$(PLAT)\datrie.lib $(TRIETOOL_DEP_LIBS) -out:$@ $(trietool_OBJS)
	@-if exist $@.manifest mt /manifest $@.manifest /outputresource:$@;1

$(datrie_tests):
	link $(LDFLAGS) $** -out:$@
	@-if exist $@.manifest mt /manifest $@.manifest /outputresource:$@;1

clean:
	@-del /f /q vs$(PDBVER)\$(CFG)\$(PLAT)\datrie-0.2.pc
	@-del /f /q vs$(PDBVER)\$(CFG)\$(PLAT)\*.exe
	@-del /f /q vs$(PDBVER)\$(CFG)\$(PLAT)\*.dll
	@-del /f /q vs$(PDBVER)\$(CFG)\$(PLAT)\*.pdb
	@-del /f /q vs$(PDBVER)\$(CFG)\$(PLAT)\*.ilk
	@-del /f /q vs$(PDBVER)\$(CFG)\$(PLAT)\*.exp
	@-del /f /q vs$(PDBVER)\$(CFG)\$(PLAT)\*.lib
	@-if exist vs$(PDBVER)\$(CFG)\$(PLAT)\datrie-tests\ del /f /q vs$(PDBVER)\$(CFG)\$(PLAT)\datrie-tests\vc$(PDBVER)0.pdb
	@-if exist vs$(PDBVER)\$(CFG)\$(PLAT)\datrie-tests\ del /f /q vs$(PDBVER)\$(CFG)\$(PLAT)\datrie-tests\*.obj
	@-if exist vs$(PDBVER)\$(CFG)\$(PLAT)\datrie-tests\ rd vs$(PDBVER)\$(CFG)\$(PLAT)\datrie-tests
	@-if exist vs$(PDBVER)\$(CFG)\$(PLAT)\trietool\ del /f /q vs$(PDBVER)\$(CFG)\$(PLAT)\trietool\vc$(PDBVER)0.pdb
	@-if exist vs$(PDBVER)\$(CFG)\$(PLAT)\trietool\ del /f /q vs$(PDBVER)\$(CFG)\$(PLAT)\trietool\*.obj
	@-if exist vs$(PDBVER)\$(CFG)\$(PLAT)\trietool\ del /f /q vs$(PDBVER)\$(CFG)\$(PLAT)\trietool\config.h
	@-if exist vs$(PDBVER)\$(CFG)\$(PLAT)\trietool\ rd vs$(PDBVER)\$(CFG)\$(PLAT)\trietool
	@-del /f /q vs$(PDBVER)\$(CFG)\$(PLAT)\datrie\vc$(PDBVER)0.pdb
	@-del /f /q vs$(PDBVER)\$(CFG)\$(PLAT)\datrie\datrie.def
	@-del /f /q vs$(PDBVER)\$(CFG)\$(PLAT)\datrie\*.obj
	@-rd vs$(PDBVER)\$(CFG)\$(PLAT)\datrie
