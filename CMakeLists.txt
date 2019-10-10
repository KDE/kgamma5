cmake_minimum_required(VERSION 3.0)

project(kgamma)
set(PROJECT_VERSION "5.17.0")

set(QT_MIN_VERSION "5.12.0")
set(KF5_MIN_VERSION "5.62.0")

find_package(ECM ${KF5_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)
include(FeatureSummary)

find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS Core Gui Widgets)

find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS
    Config
    ConfigWidgets
    DocTools
    I18n
)

find_package(X11 REQUIRED)

# X11_xf86vmode discovery is done by FindX11
add_feature_info("X11_xf86vmode" X11_xf86vmode_FOUND "Required to build kgamma.")

# check_include_files(strstream HAVE_STRSTREAM_H)
configure_file(config-kgamma.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-kgamma.h)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
if (EXISTS "${CMAKE_SOURCE_DIR}/.git")
   add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x060000)
endif()

add_subdirectory(kcmkgamma)
# add_subdirectory(xf86gammacfg)
add_subdirectory(doc)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
