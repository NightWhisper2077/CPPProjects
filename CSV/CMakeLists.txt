cmake_minimum_required(VERSION 3.24)
project(CSV)

set(CMAKE_CXX_STANDARD 17)

add_executable(CSV main.cpp CSVParser.h)
