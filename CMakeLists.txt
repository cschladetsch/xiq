cmake_minimum_required(VERSION 3.11)

project(Xiq)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED OFF)
set(Boost_USE_STATIC_RUNTIME OFF)
set(BOOST_ROOT C:/dev/boost/boost_1_67_0/boost_1_67_0)
find_package(Boost 1.40)

include_directories(${Boost_INCLUDE_DIR})
include_directories(Include)

#### #file(GLOB Libs External/*.lib)
file(GLOB Sources Source/*.cpp)

add_executable(xiq ${Sources})
target_link_libraries(xiq ${Libs} )

