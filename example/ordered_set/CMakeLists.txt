cmake_minimum_required(VERSION 3.14)

project(claw-ordered_set-example)

if(CMAKE_COMPILER_IS_GNUCXX)
  add_compile_options(-Wall -pedantic)
endif()

find_package(claw)

add_executable(ex-ordered_set main.cpp)
target_link_libraries(ex-ordered_set claw_core)
