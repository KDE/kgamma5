check_include_files( strstream HAVE_STRSTREAM_H )
configure_file (config-kgamma.h.cmake
${CMAKE_CURRENT_BINARY_DIR}/config-kgamma.h)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory( kcmkgamma ) 
add_subdirectory( xf86gammacfg ) 
