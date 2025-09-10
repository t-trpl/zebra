cmake_minimum_required(VERSION 3.10)
project(zebra CXX)

set(CMAKE_BUILD_PARALLEL_LEVEL 2)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(CheckIncludeFileCXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(${PROJECT_NAME} main.cc)

# CXX FLAGS
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wunused -O2")
message(STATUS "CXX Flags: ${CMAKE_CXX_FLAGS}")

aux_source_directory(src SRC)

target_include_directories(${PROJECT_NAME}
                           PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_sources(${PROJECT_NAME}
               PRIVATE
               ${SRC})
