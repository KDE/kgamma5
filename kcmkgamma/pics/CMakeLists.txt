



########### install files ###############

install( FILES background.png cmyscale.png darkgrey.png greyscale.png lightgrey.png midgrey.png  	rgbscale.png  DESTINATION  ${DATA_INSTALL_DIR}/kgamma/pics )

kde4_install_icons( ${ICON_INSTALL_DIR}   )




#original Makefile.am contents follow:

#
#img_DATA = background.png cmyscale.png darkgrey.png greyscale.png lightgrey.png midgrey.png \
#	rgbscale.png
#imgdir = $(kde_datadir)/kgamma/pics
#
#KDE_ICON = kgamma
#
