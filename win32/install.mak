# NMake Makefile snippet for copying the built libraries, utilities and headers to
# a path under $(PREFIX).

install: all
	@if not exist $(PREFIX)\bin\ mkdir $(PREFIX)\bin
	@if not exist $(PREFIX)\lib\pkgconfig\ mkdir $(PREFIX)\lib\pkgconfig
	@if not exist $(PREFIX)\include\datrie @mkdir $(PREFIX)\include\datrie
	@copy /b vs$(PDBVER)\$(CFG)\$(PLAT)\datrie.dll $(PREFIX)\bin
	@copy /b vs$(PDBVER)\$(CFG)\$(PLAT)\datrie.pdb $(PREFIX)\bin
	@copy /b vs$(PDBVER)\$(CFG)\$(PLAT)\datrie.lib $(PREFIX)\lib
	@if exist  vs$(PDBVER)\$(CFG)\$(PLAT)\trietool.exe copy /b vs$(PDBVER)\$(CFG)\$(PLAT)\trietool.exe $(PREFIX)\bin
	@if exist  vs$(PDBVER)\$(CFG)\$(PLAT)\trietool.exe copy /b vs$(PDBVER)\$(CFG)\$(PLAT)\trietool.exe $(PREFIX)\bin\trietool-0.2.exe
	@if exist  vs$(PDBVER)\$(CFG)\$(PLAT)\trietool.exe copy /b vs$(PDBVER)\$(CFG)\$(PLAT)\trietool.pdb $(PREFIX)\bin
	@for %f in (typedefs triedefs alpha-map trie) do @copy ..\datrie\%f.h $(PREFIX)\include\datrie\%f.h
	@rem Copy the generated pkg-config file
	@copy /y vs$(PDBVER)\$(CFG)\$(PLAT)\datrie-0.2.pc $(PREFIX)\lib\pkgconfig
