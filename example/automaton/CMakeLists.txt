cmake_minimum_required(VERSION 3.14)

project(claw-automaton-example)

if(CMAKE_COMPILER_IS_GNUCXX)
  add_compile_options(-Wall -pedantic)
endif()

find_package(claw)

add_executable(ex-automaton main.cpp)
target_link_libraries(ex-automaton claw_core)
