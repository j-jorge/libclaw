cmake_minimum_required(VERSION 3.14)

project(claw-avl-example)

if(CMAKE_COMPILER_IS_GNUCXX)
  add_compile_options(-Wall -pedantic)
endif()

find_package(claw)

add_executable(ex-avl main.cpp)
target_link_libraries(ex-avl claw_core)

add_executable(ex-vs std_vs_claw.cpp)
target_link_libraries(ex-vs claw_core)
