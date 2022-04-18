cmake_minimum_required(VERSION 3.14)

project(claw-graph-example)

if(CMAKE_COMPILER_IS_GNUCXX)
  add_compile_options(-Wall -pedantic)
endif()

find_package(claw)

add_executable(ex-graph main.cpp)
target_link_libraries(ex-graph claw_core)

add_executable(ex-algo algos.cpp)
target_link_libraries(ex-algo claw_core)

add_executable(mk_test mk_test.cpp)
