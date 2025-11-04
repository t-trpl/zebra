cmake_minimum_required(VERSION 3.10)
project(zebra CXX)

set(CMAKE_BUILD_PARALLEL_LEVEL 2)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

aux_source_directory(src SRC_LIST)

add_executable(${PROJECT_NAME}
    main.cc
    ${SRC_LIST}
)

target_compile_options(${PROJECT_NAME}
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wunused
        -Werror
        -O2
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

get_target_property(TARGET_FLAGS ${PROJECT_NAME} COMPILE_OPTIONS)
message(STATUS "Target compile options: ${TARGET_FLAGS}")
