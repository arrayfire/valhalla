cmake_minimum_required(VERSION 2.8)
project(VALHALLA)

option(BUILD_ARRAYFIRE "Build the ArrayFire files" ON)

ADD_DEFINITIONS("-std=c++11")

if (${BUILD_ARRAYFIRE})
    add_subdirectory(src/arrayfire)
endif()
