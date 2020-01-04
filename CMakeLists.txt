cmake_minimum_required(VERSION 3.15)
project(Boost_Echo_Client)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)
include_directories(bin)
include_directories(include)
include_directories(src)
find_package (Threads)
add_executable(Boost_Echo_Client

        include/connectionHandler.h
        src/connectionHandler.cpp
        src/echoClient.cpp
        makefile src/Books.cpp include/Books.h)
target_link_libraries (Boost_Echo_Client ${CMAKE_THREAD_LIBS_INIT})