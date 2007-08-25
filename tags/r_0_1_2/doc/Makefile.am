if ENABLE_DOXYGEN_DOC

all-local: doxygen.stamp

doxygen.stamp: $(top_srcdir)/datrie/*.h
	$(DOXYGEN)
	touch doxygen.stamp

install-data-local: install-html install-man

install-html:
	$(INSTALL) -d $(DESTDIR)$(htmldocdir)
	$(INSTALL_DATA) html/* $(DESTDIR)$(htmldocdir)

install-man:
	$(INSTALL) -d $(DESTDIR)$(mandir)/man3
	$(INSTALL_DATA) man/man3/* $(DESTDIR)$(mandir)/man3

uninstall-local:
	rm -rf $(DESTDIR)$(htmldocdir)
	rm -rf $(DESTDIR)$(mandir)/man3

clean-local:
	rm -rf html man
	rm -f doxygen.stamp

else

install-data-local:
uninstall-local:
clean-local:

endif

