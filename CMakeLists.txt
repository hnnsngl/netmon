cmake_minimum_required (VERSION 2.6)
PROJECT(NetMon)

# SET(CMAKE_VERBOSE_MAKEFILE true)

# looking for boost
find_package( Boost COMPONENTS program_options system regex )
link_directories    ( ${Boost_LIBRARY_DIRS} )
include_directories ( ${Boost_INCLUDE_DIRS} )


# set compiler options

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -std=c++0x")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS} -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} -O2")

link_directories    ( ${Boost_LIBRARY_DIRS} )
include_directories ( ${Boost_INCLUDE_DIRS} )

add_library(socket src/socket.cpp)
set(CORELIBS socket ${Boost_LIBRARIES})


# include project parts
include_directories ( include )

add_subdirectory(src)
add_subdirectory(bin)
add_subdirectory(netmon-qt)
