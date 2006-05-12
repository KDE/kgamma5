



########### next target ###############

set(xf86gammacfg_SRCS xf86gammacfg.cpp )

kde4_automoc(${xf86gammacfg_SRCS})

kde4_add_executable(xf86gammacfg ${xf86gammacfg_SRCS})

target_link_libraries(xf86gammacfg  ${KDE4_KDECORE_LIBS} )

install_targets(/bin xf86gammacfg )


########### install files ###############





#original Makefile.am contents follow:

#
#INCLUDES= $(all_includes)
#
#bin_PROGRAMS = xf86gammacfg
#xf86gammacfg_SOURCES = xf86gammacfg.cpp 
#xf86gammacfg_LDADD   = 
#xf86gammacfg_METASOURCES =  AUTO
#noinst_HEADERS =
#
#EXTRA_DIST = xf86gammacfg.cpp 
#
