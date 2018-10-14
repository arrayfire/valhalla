cmake_minimum_required(VERSION 3.0)
project(VALHALLA)

INCLUDE (FindPackageHandleStandardArgs)
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

option(BUILD_ArrayFire "Build the ArrayFire benchmark files" ON)
option(BUILD_Thrust "Build the Thrust benchmark files" OFF)
option(BUILD_BoostCompute "Build the Boost Compute benchmark files" OFF)
option(BUILD_HSABolt "Build the HSA Bolt files" OFF)
option(BUILD_TBB "Build the TBB files" OFF)


SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_STANDARD_REQUIRED)

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

if (${BUILD_HSABolt})
    add_subdirectory(src/hsa.bolt)
endif()

if (${BUILD_TBB})
    add_subdirectory(src/tbb)
endif()
