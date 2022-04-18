cmake_minimum_required(VERSION 3.14)

project(claw-codec-example)

if(CMAKE_COMPILER_IS_GNUCXX)
  add_compile_options(-Wall -pedantic)
endif()

find_package(claw)

add_executable(ex-compress compress.cpp)
target_link_libraries(ex-compress claw_core)

add_executable(ex-decompress decompress.cpp)
target_link_libraries(ex-decompress claw_core)
