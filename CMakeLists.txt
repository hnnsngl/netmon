cmake_minimum_required (VERSION 2.6)
PROJECT(NetMon)

# SET(CMAKE_VERBOSE_MAKEFILE true)

# looking for boost

#path to boost with cmake -DBOOST_ROOT=/past/to/boost
if(BOOST_ROOT)
  set(BOOST_ROOT ${BOOST_ROOT})
  message("search for boost dir in " ${BOOST_ROOT})
endif()
if(BOOST_INCLUDEDIR)
  set(BOOST_INCLUDEDIR ${BOOST_INCLUDEDIR})
  message("search for boost include dir in " ${BOOST_INCLUDEDIR})
endif()
if(BOOST_LIBRARYDIR)
  set(BOOST_LIBRARYDIR ${BOOST_LIBRARYDIR})
  message("search for boost library dir in " ${BOOST_LIBRARYDIR})
endif()
find_package(Boost COMPONENTS program_options system regex REQUIRED)

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
