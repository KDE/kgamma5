
########### next target ###############

set(xf86gammacfg_SRCS xf86gammacfg.cpp )


kde4_add_executable(xf86gammacfg ${xf86gammacfg_SRCS})

target_link_libraries(xf86gammacfg  ${KDE4_KDECORE_LIBS} )

install(TARGETS xf86gammacfg  DESTINATION ${BIN_INSTALL_DIR} )


