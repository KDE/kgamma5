
INCLUDES = $(all_includes)

kde_module_LTLIBRARIES = libkcm_kgamma.la
libkcm_kgamma_la_METASOURCES=AUTO
libkcm_kgamma_la_SOURCES = xf86configpath.cpp xvidextwrap.cpp displaynumber.cpp gammactrl.cpp kgamma.cpp 
libkcm_kgamma_la_LIBADD = $(LIBVM) $(LIB_KDEUI) $(LIB_KDECORE) $(LIB_QT)
libkcm_kgamma_la_LDFLAGS = $(all_libraries) -module $(KDE_PLUGIN)

SUBDIRS = pics 


xdg_apps_DATA = kgamma.desktop

messages:
	$(XGETTEXT) $(libkcm_kgamma_la_SOURCES) -o $(podir)/kgamma.pot

KDE_OPTIONS = nofinal
