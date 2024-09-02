cmake_minimum_required(VERSION 3.27)
project(chatServer)

set(CMAKE_CXX_STANDARD 17)
add_executable(chatServer server.cpp
        translate.cpp)
