cmake_minimum_required(VERSION 3.22)
project(utils)

add_executable(optional optional.cpp)
add_executable(services services.cpp)

target_link_libraries(optional utils)
target_link_libraries(services utils)
