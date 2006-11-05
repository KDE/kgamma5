add_subdirectory( pics ) 




########### next target ###############

set(kcm_kgamma_PART_SRCS 
   xf86configpath.cpp 
   xvidextwrap.cpp 
   displaynumber.cpp 
   gammactrl.cpp 
   kgamma.cpp )

kde4_automoc(${kcm_kgamma_PART_SRCS})

kde4_add_plugin(kcm_kgamma ${kcm_kgamma_PART_SRCS})


target_link_libraries(kcm_kgamma Xxf86vm ${KDE4_KDEUI_LIBS} )

install(TARGETS kcm_kgamma  DESTINATION ${PLUGIN_INSTALL_DIR} )


########### install files ###############

install( FILES kgamma.desktop  DESTINATION  ${XDG_APPS_DIR} )




#original Makefile.am contents follow:

#
#INCLUDES = $(all_includes)
#
#kde_module_LTLIBRARIES = kcm_kgamma.la
#kcm_kgamma_la_METASOURCES=AUTO
#kcm_kgamma_la_SOURCES = xf86configpath.cpp xvidextwrap.cpp displaynumber.cpp \
#                        gammactrl.cpp kgamma.cpp 
#kcm_kgamma_la_LIBADD = $(LIBVM) $(LIB_KDEUI) $(LIB_KDECORE) $(LIB_QT)
#kcm_kgamma_la_LDFLAGS = $(all_libraries) -module $(KDE_PLUGIN)
#
#SUBDIRS = pics 
#
#xdg_apps_DATA = kgamma.desktop
#
#messages:
#	$(XGETTEXT) *.cpp -o $(podir)/kgamma.pot
#
#KDE_OPTIONS = nofinal
