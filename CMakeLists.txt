kde4_header()

add_subdirectory( kcmkgamma ) 
add_subdirectory( xf86gammacfg ) 


check_include_files( sstream HAVE_SSTREAM )

########### install files ###############


kde4_footer()



#original Makefile.am contents follow:

#SUBDIRS = kcmkgamma xf86gammacfg
#
