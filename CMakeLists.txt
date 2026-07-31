cmake_minimum_required(VERSION 4.0.0)
project(untitled LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror -Wall -pedantic")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fsanitize=address")

option(UNTITLED_BUILD_TESTS OFF)
option(SY_DS_USE_SHARED "Build `sy_ds` library as shared" OFF)

find_package(sy_ds QUIET)

if(sy_ds_FOUND)
        message(STATUS "Found pre-installed `sy_ds` library.")
else()
        message(STATUS "`sy_ds` not found. Building from `git subtree` soruce.")
        set(sy_ds_SHARED ${SY_DS_USE_SHARED})
        add_subdirectory(${CMAKE_SOURCE_DIR}/sy_ds)
endif()

find_package(SDL3 CONFIG REQUIRED)
find_package(Vulkan REQUIRED)

# Add include `sy_ds` include directories
include_directories(
        ${CMAKE_SOURCE_DIR}/sy_ds/include
)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PUBLIC
	${CMAKE_SOURCE_DIR}/src/main.c
)

target_include_directories(${PROJECT_NAME} PUBLIC
	${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
        SDL3::SDL3 
        Vulkan::Vulkan
        sy_ds::sy_ds
)



