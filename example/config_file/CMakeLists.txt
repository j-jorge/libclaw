cmake_minimum_required(VERSION 3.14)

project(claw-config_file-example)

if(CMAKE_COMPILER_IS_GNUCXX)
  add_compile_options(-Wall -pedantic)
endif()

find_package(claw)

add_executable(ex-config main.cpp application.cpp)
target_link_libraries(
  ex-config
  claw_configuration_file
  claw_application
  )
