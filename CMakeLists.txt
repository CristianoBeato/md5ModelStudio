cmake_minimum_required(VERSION 3.13)

project( md5ModelStudio )

## STD 17
set( CMAKE_CXX_STANDARD 17 )

find_package( SDL3 REQUIRED )
find_package( SDL3_image REQUIRED )
find_package( SDL3_ttf REQUIRED )

include_directories( ${CMAKE_SOURCE_DIR} )
include_directories( ${CMAKE_SOURCE_DIR}/contrib/glm )
include_directories( ${CMAKE_SOURCE_DIR}/contrib/crglLib )
include_directories( ${CMAKE_SOURCE_DIR}/contrib/gwen )

add_subdirectory( contrib/gwen )
add_subdirectory( contrib/crglLib )



set( MD5MESH_SOURCE 
    ${CMAKE_SOURCE_DIR}/md5Mesh/md5Anim.cpp
    ${CMAKE_SOURCE_DIR}/md5Mesh/md5Anim.hpp
    ${CMAKE_SOURCE_DIR}/md5Mesh/md5Joint.cpp
    ${CMAKE_SOURCE_DIR}/md5Mesh/md5Joint.hpp
    ${CMAKE_SOURCE_DIR}/md5Mesh/md5Mesh.cpp
    ${CMAKE_SOURCE_DIR}/md5Mesh/md5Mesh.hpp
    ${CMAKE_SOURCE_DIR}/md5Mesh/md5Model.cpp
    ${CMAKE_SOURCE_DIR}/md5Mesh/md5Model.hpp
    )

set( MAIN_SOURCES 
    ${CMAKE_SOURCE_DIR}/precompiled.hpp
    ${CMAKE_SOURCE_DIR}/main.cpp
    ${CMAKE_SOURCE_DIR}/main.hpp
    ${CMAKE_SOURCE_DIR}/ViewPort.cpp
    ${CMAKE_SOURCE_DIR}/ViewPort.hpp
    )

set( SOURCES
    ${CRGL_SOURCE}
    ${MD5MESH_SOURCE}
    ${MAIN_SOURCES}
    )

if( CMAKE_COMPILER_IS_GNUCC OR CMAKE_C_COMPILER_ID STREQUAL "Clang" )
	add_definitions(-Wnarrowing)
endif()

add_executable( md5ModelStudio ${SOURCES} )
target_link_libraries( md5ModelStudio PUBLIC Gwen crglLib SDL3::SDL3 SDL3_image::SDL3_image SDL3_ttf::SDL3_ttf )
target_precompile_headers( md5ModelStudio PUBLIC "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/precompiled.hpp>" )
