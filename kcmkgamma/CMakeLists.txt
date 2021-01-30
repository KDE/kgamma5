add_definitions(-DTRANSLATION_DOMAIN=\"kcmkgamma\")

add_subdirectory(pics)

set(kcm_kgamma_PART_SRCS
   xf86configpath.cpp
   xvidextwrap.cpp
   displaynumber.cpp
   gammactrl.cpp
   kgamma.cpp
)

add_library(kcm_kgamma MODULE ${kcm_kgamma_PART_SRCS})

target_link_libraries(kcm_kgamma ${X11_Xxf86vm_LIB} ${X11_LIBRARIES} Qt::Core Qt::Widgets KF5::ConfigCore KF5::ConfigWidgets KF5::I18n)

install(TARGETS kcm_kgamma DESTINATION ${KDE_INSTALL_PLUGINDIR})

install(FILES kgamma.desktop DESTINATION ${KDE_INSTALL_KSERVICES5DIR})

