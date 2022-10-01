cmake_minimum_required(VERSION 3.22)
project(utils)

add_executable(optional optional.cpp)
target_link_libraries(optional utils)
