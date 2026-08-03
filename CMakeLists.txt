cmake_minimum_required(VERSION 4.0.0)
project(sy_ds LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror -Wall -pedantic")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fsanitize=address")

option(SY_DS_BUILD_TESTS "Build tests for data structures against varying metrics" OFF)
option(SY_DS_BUILD_SHARED_LIB "Build shared library for using data structures in other projects" OFF)

set(SY_DS_SOURCE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/SY_Result.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/SY_Allocator.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/SY_Vector.c
)

if(SY_DS_BUILD_SHARED_LIB)
        add_library(${PROJECT_NAME} SHARED ${SY_DS_SOURCE_FILES})
else()
        add_library(${PROJECT_NAME} STATIC ${SY_DS_SOURCE_FILES})
endif()

add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}Targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT ${PROJECT_NAME}Targets
        FILE ${PROJECT_NAME}Targets.cmake
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
