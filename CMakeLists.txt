project(kgamma)
cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)
set(PROJECT_VERSION "5.2.90")
set(QT_MIN_VERSION "5.4.0")
find_package(ECM 1.0.0 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(ECMInstallIcons)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)
include(FeatureSummary)

find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS Core Gui Widgets X11Extras)

find_package(KF5 REQUIRED COMPONENTS
    KDELibs4Support
)

find_package(X11 REQUIRED)

# X11_xf86vmode discovery is done by FindX11
# macro_log_feature(X11_xf86vmode_FOUND "X11 Video Mode Extension (xf86vm)" "Required to build kgamma." "http://www.x.org" TRUE "" "")

# check_include_files(strstream HAVE_STRSTREAM_H)
configure_file(config-kgamma.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-kgamma.h)


include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_definitions(-DTRANSLATION_DOMAIN=\"kcm_gamma\")

add_subdirectory(kcmkgamma)
# add_subdirectory(xf86gammacfg)
add_subdirectory(doc)

# macro_display_feature_log()
