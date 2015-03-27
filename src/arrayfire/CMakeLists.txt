FIND_PACKAGE(ArrayFire REQUIRED)
INCLUDE_DIRECTORIES(${ArrayFire_INCLUDE_DIRS})

# A macro to build an ArrayFire example
# For most uses only FIND_PACKAGE(ArrayFire REQUIRED), ADD_EXECUTABLE(...)
# and TARGET_LINK_LIBRARIES(... ${ARRAYFIRE_LIBRARIES}) are needed
MACRO(BUILD_FUNC FUNC_NAME FUNC_SOURCE BACKEND_NAME BACKEND_LIBRARIES)

    ADD_EXECUTABLE(af_${FUNC_NAME}_${BACKEND_NAME} ${FUNC_SOURCE})

    TARGET_LINK_LIBRARIES(af_${FUNC_NAME}_${BACKEND_NAME}
                         ${BACKEND_LIBRARIES} )

    SET_TARGET_PROPERTIES(af_${FUNC_NAME}_${BACKEND_NAME}
        PROPERTIES
        OUTPUT_NAME ${FUNC_NAME}_${BACKEND_NAME})
ENDMACRO()

# Collect the source
FILE(GLOB FILES "*.cpp")

FOREACH(FILE ${FILES})
    GET_FILENAME_COMPONENT(FUNC ${FILE} NAME_WE)
    GET_FILENAME_COMPONENT(FULL_DIR_NAME ${FILE} PATH)
    GET_FILENAME_COMPONENT(DIR_NAME ${FULL_DIR_NAME} NAME)

    # Next we build each example using every backend.
    if(${ArrayFire_CPU_FOUND})  # variable defined by FIND(ArrayFire ...)
        BUILD_FUNC(${FUNC} ${FILE} cpu ${ArrayFire_CPU_LIBRARIES})
    elseif(TARGET afcpu)        # variable defined by the ArrayFire build tree
        BUILD_FUNC(${FUNC} ${FILE} cpu afcpu)
    endif()

    if(${ArrayFire_CUDA_FOUND})  # variable defined by FIND(ArrayFire ...)
        BUILD_FUNC(${FUNC} ${FILE} cuda ${ArrayFire_CUDA_LIBRARIES})
    elseif(TARGET afcuda)        # variable defined by the ArrayFire build tree
        BUILD_FUNC(${FUNC} ${FILE} cuda afcuda)
    endif()

    if(${ArrayFire_OpenCL_FOUND})  # variable defined by FIND(ArrayFire ...)
        BUILD_FUNC(${FUNC} ${FILE} opencl ${ArrayFire_OpenCL_LIBRARIES})
    elseif(TARGET afopencl)        # variable defined by the ArrayFire build tree
        BUILD_FUNC(${FUNC} ${FILE} opencl afopencl)
    endif()
ENDFOREACH()
