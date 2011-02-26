project(kgamma)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/modules )

find_package(KDE4 REQUIRED)
include(KDE4Defaults)
include(MacroLibrary)
include(MacroOptionalAddSubdirectory)

check_include_files( strstream HAVE_STRSTREAM_H )
configure_file (config-kgamma.h.cmake
${CMAKE_CURRENT_BINARY_DIR}/config-kgamma.h)

add_definitions (${QT_DEFINITIONS} ${QT_QTDBUS_DEFINITIONS} ${KDE4_DEFINITIONS} )
include_directories (${QDBUS_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${KDE4_INCLUDES})

add_subdirectory( kcmkgamma ) 
add_subdirectory( xf86gammacfg )
add_subdirectory( doc )
