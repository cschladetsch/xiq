cmake_minimum_required(VERSION 3.7)

set(PROJ_NAME xiq)
project(${PROJ_NAME})

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories("Include")

file(GLOB SOURCES "Source/*.cpp")
add_executable(${PROJ_NAME} ${SOURCES})
target_link_libraries(${PROJ_NAME} ${SDL2_LIBRARIES})

