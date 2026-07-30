cmake_minimum_required(VERSION 4.0.0)
project(untitled LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror -Wall -pedantic")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fsanitize=address")

option(BUILD_TESTS OFF)

find_package(SDL3 CONFIG REQUIRED)
find_package(Vulkan REQUIRED)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PUBLIC
	${CMAKE_SOURCE_DIR}/src/main.c
        ${CMAKE_SOURCE_DIR}/src/SY_Result.c
        ${CMAKE_SOURCE_DIR}/src/SY_Allocator.c
        ${CMAKE_SOURCE_DIR}/src/SY_Vector.c
)

target_include_directories(${PROJECT_NAME} PUBLIC
	${CMAKE_SOURCE_DIR}/include
)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3 Vulkan::Vulkan)

