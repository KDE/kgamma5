
INCLUDES = $(all_includes)

kde_module_LTLIBRARIES = kcm_kgamma.la
kcm_kgamma_la_METASOURCES=AUTO
kcm_kgamma_la_SOURCES = xf86configpath.cpp xvidextwrap.cpp displaynumber.cpp \
                        gammactrl.cpp kgamma.cpp 
kcm_kgamma_la_LIBADD = $(LIBVM) $(LIB_KDEUI) $(LIB_KDECORE) $(LIB_QT)
kcm_kgamma_la_LDFLAGS = $(all_libraries) -module $(KDE_PLUGIN)

SUBDIRS = pics 

xdg_apps_DATA = kgamma.desktop

messages:
	$(XGETTEXT) *.cpp -o $(podir)/kgamma.pot

KDE_OPTIONS = nofinal
