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
find_package(Boost COMPONENTS program_options system filesystem REQUIRED)

# set compiler options

set(CMAKE_CXX_COMPILER mpic++)
add_definitions(-pedantic -Wall -O2 -g -funroll-loops -std=c++0x -static )

link_directories    ( ${Boost_LIBRARY_DIRS} )
include_directories ( ${Boost_INCLUDE_DIRS} )

# include general stuff

include_directories ( src )

add_subdirectory(src)
add_subdirectory(bin)
add_subdirectory(netmon-qt)
