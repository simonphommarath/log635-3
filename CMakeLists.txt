cmake_minimum_required (VERSION 3.7.2)
project (ai)
set (CMAKE_CXX_STANDARD 14)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(ai main.cpp
	nn.cpp)
