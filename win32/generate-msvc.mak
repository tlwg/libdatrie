# NMake Makefile portion for code generation and
# intermediate build directory creation
# Items in here should not need to be edited unless
# one is maintaining the NMake build files.

vs$(PDBVER)\$(CFG)\$(PLAT)\datrie\datrie.def: vs$(PDBVER)\$(CFG)\$(PLAT)\datrie ..\datrie\libdatrie.def
	@echo Generating $@...
	@echo EXPORTS>$@
	@type ..\datrie\libdatrie.def>>$@

# Generate a default config.h
vs$(PDBVER)\$(CFG)\$(PLAT)\trietool\config.h: vs$(PDBVER)\$(CFG)\$(PLAT)\trietool
	@echo Generating $@...
	@echo #define HAVE_LOCALE_CHARSET 1>$@
	@echo #define VERSION "$(DATRIE_VERSION)">>$@

# Create the build directories
vs$(PDBVER)\$(CFG)\$(PLAT)\datrie	\
vs$(PDBVER)\$(CFG)\$(PLAT)\trietool	\
vs$(PDBVER)\$(CFG)\$(PLAT)\datrie-tests:
	@-mkdir $@

# Generate pkg-config files
vs$(PDBVER)\$(CFG)\$(PLAT)\datrie-0.2.pc:
	@call create-pc.bat $@ $(DATRIE_VERSION) $(PREFIX)

