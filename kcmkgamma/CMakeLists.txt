add_definitions(-DTRANSLATION_DOMAIN=\"kcmkgamma\")

add_subdirectory(pics)

set(kcm_kgamma_PART_SRCS
   xf86configpath.cpp xf86configpath.h
   xvidextwrap.cpp xvidextwrap.h
   displaynumber.cpp displaynumber.h
   gammactrl.cpp gammactrl.h
   kgamma.cpp kgamma.h
)

add_library(kcm_kgamma MODULE ${kcm_kgamma_PART_SRCS})

target_link_libraries(kcm_kgamma ${X11_Xxf86vm_LIB} ${X11_LIBRARIES} Qt${QT_MAJOR_VERSION}::Core Qt${QT_MAJOR_VERSION}::Widgets KF5::ConfigCore KF5::ConfigWidgets KF5::I18n)

kcoreaddons_desktop_to_json(kcm_kgamma kgamma.desktop)
target_link_libraries(kcm_kgamma ${X11_Xxf86vm_LIB} ${X11_LIBRARIES} Qt${QT_MAJOR_VERSION}::Core Qt${QT_MAJOR_VERSION}::Widgets KF5::ConfigCore KF5::ConfigWidgets KF5::I18n)

add_custom_command(TARGET kcm_kgamma POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E create_symlink ../kcms/systemsettings/kcm_kgamma.so kcm_kgamma_init.so)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/kcm_kgamma_init.so DESTINATION ${KDE_INSTALL_PLUGINDIR}/plasma/kcminit)
install(TARGETS kcm_kgamma DESTINATION ${KDE_INSTALL_PLUGINDIR}/plasma/kcms/systemsettings)
