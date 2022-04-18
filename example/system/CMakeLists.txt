cmake_minimum_required(VERSION 3.14)

project(claw-system-example)

if(CMAKE_COMPILER_IS_GNUCXX)
  add_compile_options(-Wall -pedantic)
endif()

find_package(claw)

add_executable(ex-system main.cpp)
target_link_libraries(ex-system claw_core)
