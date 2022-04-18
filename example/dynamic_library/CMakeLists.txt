cmake_minimum_required(VERSION 3.14)

project(claw-dynamic_library-example)

if(CMAKE_COMPILER_IS_GNUCXX)
  add_compile_options(-Wall -pedantic)
endif()

find_package(claw)

add_library(func SHARED func.cpp)
add_executable(ex-dynamic_library main.cpp)

target_link_libraries(ex-dynamic_library claw_dynamic_library)
