project(kgamma)

find_package(KDE4 REQUIRED)
include(KDE4Defaults)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories (${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${KDE4_INCLUDES})

check_include_files( strstream HAVE_STRSTREAM_H )
configure_file (config-kgamma.h.cmake
${CMAKE_CURRENT_BINARY_DIR}/config-kgamma.h)

add_subdirectory( kcmkgamma ) 
add_subdirectory( xf86gammacfg ) 
