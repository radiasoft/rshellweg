cmake_minimum_required(VERSION 2.8)

project (libHellweg2D)
set(CMAKE_BUILD_TYPE Release)

add_definitions(-DRADIA)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

add_library(libHellweg2D SHARED ../BeamSolver.cpp AnsiString.cpp TStringList.cpp)
