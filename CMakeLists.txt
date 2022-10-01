cmake_minimum_required(VERSION 3.22)
project(utils)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    message("-- Building in debug mode")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -O0 -ggdb3 -fsized-deallocation -Werror")
else()
    message("-- Building in release mode")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -Ofast -fsized-deallocation -Werror")
endif()

add_library(utils STATIC lib/error.h src/error.cpp lib/meta/concepts.h lib/interface/service_provider.h lib/service_container.h lib/interface/service_collection.h src/service_container.cpp)
target_include_directories(utils PUBLIC lib)

add_subdirectory(example)