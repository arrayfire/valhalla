cmake_minimum_required(VERSION 2.8)
project(VALHALLA)

option(BUILD_ArrayFire "Build the ArrayFire benchmark files" ON)
option(BUILD_Thrust "Build the Thrust benchmark files" OFF)
option(BUILD_BoostCompute "Build the Boost Compute benchmark files" OFF)


ADD_DEFINITIONS("-std=c++11")
INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/include")

if (${BUILD_ArrayFire})
    add_subdirectory(src/arrayfire)
endif()

if (${BUILD_Thrust})
    add_subdirectory(src/thrust)
endif()

if (${BUILD_BoostCompute})
    add_subdirectory(src/boost.compute)
endif()
