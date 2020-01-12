cmake_minimum_required(VERSION 3.6)
project(Air_Condition)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11  -lboost_system -lpthread")

set(SOURCE_FILES main.cpp AirMaster.cpp AirMaster.h json.hpp
        AirRequest.cpp AirRequest.h)
add_executable(Air_Condition ${SOURCE_FILES})