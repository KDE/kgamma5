
INCLUDES = $(all_includes)

kde_module_LTLIBRARIES = libkcm_kgamma.la
libkcm_kgamma_la_METASOURCES=AUTO
libkcm_kgamma_la_SOURCES = xf86configpath.cpp xvidextwrap.cpp gammactrl.cpp kgamma.cpp 
libkcm_kgamma_la_LIBADD = $(LIBVM) $(LIB_KDEUI) $(LIB_KDECORE) $(LIB_QT)
libkcm_kgamma_la_LDFLAGS = $(all_libraries) -module $(KDE_PLUGIN)                                         

SUBDIRS = pics 

install-data-local:
	$(mkinstalldirs) $(kde_appsdir)/Settings/Peripherals/
	$(INSTALL_DATA) $(srcdir)/kgamma.desktop $(kde_appsdir)/Settings/Peripherals/kgamma.desktop

uninstall-local:
	-rm -f $(kde_appsdir)/Settings/Peripherals/kgamma.desktop

messages:
	$(XGETTEXT) $(libkcm_kgamma_la_SOURCES) -o $(podir)/kgamma.pot

KDE_OPTIONS = nofinal
