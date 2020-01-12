cmake_minimum_required(VERSION 3.6)
project(dns_relay_server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lboost_system -lpthread -lhiredis")

set(SOURCE_FILES main.cpp DNS_Relay_Server.cpp DNS_utils.cpp DNS_utils.h DNS_Database.cpp DNS_Database.h)
add_executable(dns_relay_server ${SOURCE_FILES})