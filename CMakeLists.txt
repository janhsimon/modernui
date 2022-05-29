cmake_minimum_required(VERSION 3.1)
project(modernui)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

option(MODERNUI_BUILD_TEST "Build the test program" OFF)

add_subdirectory(external)
add_subdirectory(src)

if(MODERNUI_BUILD_TEST)
  add_subdirectory(test)
endif()