cmake_minimum_required(VERSION 3.14)

project(claw-matches-example)

if(CMAKE_COMPILER_IS_GNUCXX)
  add_compile_options(-Wall -pedantic)
endif()

find_package(claw)

add_executable(ex-matches main.cpp matches.cpp matches_main.cpp)
target_link_libraries(ex-matches claw_core)
