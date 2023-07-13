cmake_minimum_required(VERSION 3.15)
project(ToolboxTest CXX)

find_package(toolbox CONFIG REQUIRED)

add_executable(example example.cpp)
target_link_libraries(example toolbox::toolbox)
