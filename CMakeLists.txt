# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(Deacon)

set(CMAKE_CXX_STANDARD 11)

# Add the main program, server, client, and error source files
add_executable(daecon main.cpp server.cpp client.cpp)
