cmake_minimum_required(VERSION 2.8.9)
project(kgamma)

find_package(KDE4 REQUIRED)
include(KDE4Defaults)
include(MacroLibrary)

find_package(X11 REQUIRED)

# X11_xf86vmode discovery is done by FindX11
macro_log_feature(X11_xf86vmode_FOUND "X11 Video Mode Extension (xf86vm)" "Required to build kgamma." "http://www.x.org" TRUE "" "")

check_include_files(strstream HAVE_STRSTREAM_H)
configure_file(config-kgamma.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-kgamma.h)

add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(${CMAKE_CURRENT_BINARY_DIR} ${KDE4_INCLUDES})

add_subdirectory(kcmkgamma)
add_subdirectory(xf86gammacfg)
add_subdirectory(doc)

macro_display_feature_log()
