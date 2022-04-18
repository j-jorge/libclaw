cmake_minimum_required(VERSION 3.14)

project(claw-sockets-example)

if(CMAKE_COMPILER_IS_GNUCXX)
  add_compile_options(-Wall -pedantic)
endif()

find_package(claw)

add_executable(ex-sockets sockets.cpp application.cpp)
target_link_libraries(
  ex-sockets
  claw_application
  claw_net
  )
