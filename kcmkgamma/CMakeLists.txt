add_subdirectory( pics ) 


########### next target ###############

set(kcm_kgamma_PART_SRCS 
   xf86configpath.cpp 
   xvidextwrap.cpp 
   displaynumber.cpp 
   gammactrl.cpp 
   kgamma.cpp )


kde4_add_plugin(kcm_kgamma ${kcm_kgamma_PART_SRCS})


target_link_libraries(kcm_kgamma Xxf86vm ${KDE4_KDEUI_LIBS}
${X11_LIBRARIES} )

install(TARGETS kcm_kgamma  DESTINATION ${PLUGIN_INSTALL_DIR} )

install(FILES kgamma.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} )




