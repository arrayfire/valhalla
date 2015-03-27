cmake_minimum_required(VERSION 2.8)
project(VALHALLA)

option(BUILD_ARRAYFIRE "Build the ArrayFire benchmark files" ON)
option(BUILD_THRUST "Build the Thrust benchmark files" OFF)

ADD_DEFINITIONS("-std=c++11")
INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/include")

if (${BUILD_ARRAYFIRE})
    add_subdirectory(src/arrayfire)
endif()

if (${BUILD_THRUST})
    add_subdirectory(src/thrust)
endif()
