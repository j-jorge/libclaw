cmake_minimum_required(VERSION 3.14)

project(claw-kmp-example)

if(CMAKE_COMPILER_IS_GNUCXX)
  add_compile_options(-Wall -pedantic)
endif()

find_package(claw)

add_executable(ex-kmp main.cpp)
target_link_libraries(ex-kmp claw_core)
