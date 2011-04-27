project(kgamma)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/modules )

find_package(KDE4 REQUIRED)
include(KDE4Defaults)
include(MacroLibrary)
include(MacroOptionalAddSubdirectory)

if(X11_FOUND)
  #X11_xf86vmode discovery is done by FindX11
  macro_log_feature(X11_xf86vmode_FOUND "X11 Video Mode Extension (xf86vm)" "Required to build kgamma." "http://www.x.org" TRUE "" "")
endif(X11_FOUND)

if(X11_FOUND AND X11_xf86vmode_FOUND)

  check_include_files( strstream HAVE_STRSTREAM_H )
  configure_file (config-kgamma.h.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/config-kgamma.h)

  add_definitions (${QT_DEFINITIONS} ${QT_QTDBUS_DEFINITIONS} ${KDE4_DEFINITIONS} )
  include_directories (${QDBUS_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${KDE4_INCLUDES})

  add_subdirectory( kcmkgamma ) 
  add_subdirectory( xf86gammacfg )
  add_subdirectory( doc )

endif()

macro_display_feature_log()
