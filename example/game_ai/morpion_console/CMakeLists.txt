cmake_minimum_required(VERSION 3.14)

project(claw-morpion-example)

if(CMAKE_COMPILER_IS_GNUCXX)
  add_compile_options(-Wall -pedantic)
endif()

find_package(claw)

add_executable(ex-morpion main.cpp morpion.cpp morpion_main.cpp)
target_link_libraries(ex-morpion claw_core)
