cmake_minimum_required(VERSION 3.15)
project(OS_lab1)

set(CMAKE_CXX_STANDARD 11)
add_executable(OS_lab1 main.cpp Shell.h ProcessManager.h Pcb.h Rcb.h)
add_executable(Test Test.cc)