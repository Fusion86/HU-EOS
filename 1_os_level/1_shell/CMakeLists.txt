cmake_minimum_required(VERSION 3.0.0)
project(baconshell VERSION 0.1.0)

add_executable(bacon shell.cpp utility.cpp)
add_definitions(-Dbaconshell_VERSION="${baconshell_VERSION}")
