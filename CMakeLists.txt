kde4_header()

add_subdirectory( kcmkgamma ) 
add_subdirectory( xf86gammacfg ) 

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############


kde4_footer()



#original Makefile.am contents follow:

#SUBDIRS = kcmkgamma xf86gammacfg
#