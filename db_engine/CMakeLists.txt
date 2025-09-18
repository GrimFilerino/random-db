cmake_minimum_required(VERSION 3.12)
project(db C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${SRC_DIR}/include)

file(GLOB_RECURSE SRC_FILES
    ${SRC_DIR}/*.c
)

add_library(grimfilerino SHARED ${SRC_FILES})

target_include_directories(grimfilerino PRIVATE
    ${INCLUDE_DIR}
)

set_target_properties(grimfilerino PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../server/libs
)
