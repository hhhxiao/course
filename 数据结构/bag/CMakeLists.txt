cmake_minimum_required(VERSION 3.14)
project(bag)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-Wall")
add_executable(bag main.cpp
        bag.h
        )
