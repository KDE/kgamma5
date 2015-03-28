add_subdirectory(pics)

set(kcm_kgamma_PART_SRCS
   xf86configpath.cpp
   xvidextwrap.cpp
   displaynumber.cpp
   gammactrl.cpp
   kgamma.cpp
)

add_library(kcm_kgamma MODULE ${kcm_kgamma_PART_SRCS})

target_link_libraries(kcm_kgamma Xxf86vm  ${X11_LIBRARIES} Qt5::Core Qt5::Widgets KF5::KDELibs4Support)

install(TARGETS kcm_kgamma DESTINATION ${PLUGIN_INSTALL_DIR})

install(FILES kgamma.desktop DESTINATION ${SERVICES_INSTALL_DIR})

