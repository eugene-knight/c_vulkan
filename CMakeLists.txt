cmake_minimum_required(VERSION 4.0.0)
project(sy_ds LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror -Wall -pedantic")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fsanitize=address")

option(SY_BUILD_TESTS "Build tests for data structures against varying metrics" OFF)
option(SY_BUILD_SHARED_LIB "Build shared library for using data structures in other projects" OFF)
option(SY_BUILD_STATIC_LIB "Build static library for using data structures in other projects" OFF)

if(SY_BUILD_TESTS AND (SY_BUILD_STATIC_LIB OR SY_BUILD_STATIC_LIB))
        message(FATAL_ERROR "BUILD_TESTS cannot be set along side BUILD_SHARED_LIB or BUILD_STATIC_LIB")
endif()

set(SY_DS_SOURCE_FILES
        ${CMAKE_SOURCE_DIR}/src/main.c
        ${CMAKE_SOURCE_DIR}/src/SY_Result.c
        ${CMAKE_SOURCE_DIR}/src/SY_Allocator.c
        ${CMAKE_SOURCE_DIR}/src/SY_Vector.c
)

if(SY_BUILD_SHARED_LIB)
        set(SY_PROJECT_NAME ${PROJECT_NAME})
        add_library(${PROJECT_NAME}_shared SHARED ${SY_DS_SOURCE_FILES})
        target_include_directories(${PROJECT_NAME}_shared PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        )
endif()

if(SY_BUILD_STATIC_LIB)
        add_library(${PROJECT_NAME}_static STATIC ${SY_DS_SOURCE_FILES})
        target_include_directories(${PROJECT_NAME}_static PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        )
endif()

if(SY_BUILD_TESTS)
        list(APPEND SY_DS_SOURCE_FILES
                ${CMAKE_SOURCE_DIR}/src/tests.c
        )
        add_executable(${PROJECT_NAME}_tests ${SY_DS_SOURCE_FILES})
        target_include_directories(${PROJECT_NAME}_tests PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        )
endif()

