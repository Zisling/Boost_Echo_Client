cmake_minimum_required(VERSION 3.15)
project(Boost_Echo_Client)

set(CMAKE_CXX_STANDARD 11)
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.65.1 COMPONENTS filesystem regex)
find_package(Boost COMPONENTS thread REQUIRED)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})

endif()
include_directories(.)
include_directories(bin)
include_directories(include)
include_directories(src)
find_package (Threads)
add_executable(Boost_Echo_Client

        include/connectionHandler.h
        src/connectionHandler.cpp
        src/echoClient.cpp
        src/Books.cpp include/Books.h
        src/UserIO.cpp include/UserIO.h
        src/SocketIO.cpp include/SocketIO.h)
target_link_libraries(Boost_Echo_Client ${Boost_LIBRARIES})
target_link_libraries (Boost_Echo_Client ${CMAKE_THREAD_LIBS_INIT})