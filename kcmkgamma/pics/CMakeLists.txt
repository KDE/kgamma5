kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kgamma/pics FILES background.png cmyscale.png darkgrey.png greyscale.png lightgrey.png midgrey.png  	rgbscale.png )

kde4_install_icons( ${ICON_INSTALL_DIR}  hicolor )

kde4_footer()



#original Makefile.am contents follow:

#
#img_DATA = background.png cmyscale.png darkgrey.png greyscale.png lightgrey.png midgrey.png \
#	rgbscale.png
#imgdir = $(kde_datadir)/kgamma/pics
#
#KDE_ICON = kgamma
#
