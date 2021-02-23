cmake_minimum_required(VERSION 3.10)

# 显式指定编译器
set(CMAKE_CXX_COMPILER "g++")
 
# 开启调试信息
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
 
 
# 开启所有警告
set(CMAKE_CXX_FLAGS "-Wall")

project(CppImplementDES)

set(CMAKE_CXX_STANDARD 11)

add_executable(CppImplementDES main.cpp des.cpp fileencryption.cpp)