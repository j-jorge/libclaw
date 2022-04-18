cmake_minimum_required(VERSION 3.14)

project(claw-memory)

if(CMAKE_COMPILER_IS_GNUCXX)
  add_compile_options(-Wall -pedantic)
endif()

find_package(claw)

add_executable(ex-memory main.cpp)
target_link_libraries(ex-memory claw_core)
