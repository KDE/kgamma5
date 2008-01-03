check_include_files( strstream HAVE_STRSTREAM_H )
configure_file (config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory( kcmkgamma ) 
message(STATUS "Need to port kcmkgamma")
add_subdirectory( xf86gammacfg ) 
