cmake_minimum_required(VERSION 3.16)

project(kgamma)
set(PROJECT_VERSION "5.25.3")

set(QT_MIN_VERSION "5.15.2")
set(KF5_MIN_VERSION "5.94")
set(KDE_COMPILERSETTINGS_LEVEL "5.82")

find_package(ECM ${KF5_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH}  ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(FeatureSummary)
include(KDEClangFormat)
include(KDEGitCommitHooks)

find_package(Qt${QT_MAJOR_VERSION} ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS Core Gui Widgets)

find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS
    Config
    ConfigWidgets
    DocTools
    I18n
)

find_package(X11 REQUIRED COMPONENTS xf86vmode)

# X11_xf86vmode discovery is done by FindX11
add_feature_info("X11_xf86vmode" X11_xf86vmode_FOUND "Required to build kgamma.")

# check_include_files(strstream HAVE_STRSTREAM_H)
configure_file(config-kgamma.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-kgamma.h)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x050f02)
add_definitions(-DKF_DISABLE_DEPRECATED_BEFORE_AND_AT=0x055900)

add_subdirectory(kcmkgamma)
# add_subdirectory(xf86gammacfg)
add_subdirectory(doc)

# add clang-format target for all our real source files
file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES *.cpp *.h)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})
kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
