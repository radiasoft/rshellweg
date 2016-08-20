cmake_minimum_required(VERSION 2.8)

project (libHellweg2D)
set(CMAKE_BUILD_TYPE Release)

find_package(Boost)

add_definitions(-DRADIA)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

add_library(libHellweg2D SHARED ../BeamSolver.cpp ../Spline.cpp ../Matrix.cpp ../Beam.cpp ../Spectrum.cpp ../ConstUnit.cpp
    AnsiString.cpp TStringList.cpp IniFiles.cpp)
