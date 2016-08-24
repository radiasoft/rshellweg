cmake_minimum_required(VERSION 2.8)

project (libHellweg2D)

set(LIB_HW_VERSION_MAJOR 0)
set(LIB_HW_VERSION_MINOR 1)
set(LIB_HW_VERSION_PATCH 0)
set(LIB_HW_VERSION_STRING ${LIB_HW_VERSION_MAJOR}.${LIB_HW_VERSION_MINOR}.${LIB_HW_VERSION_PATCH})

set(CMAKE_BUILD_TYPE Release)

find_package(Boost)

add_definitions(-DRADIA)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

add_library(Hellweg2D SHARED ../BeamSolver.cpp ../Spline.cpp ../Matrix.cpp ../Beam.cpp ../Spectrum.cpp ../ConstUnit.cpp
    AnsiString.cpp TStringList.cpp IniFiles.cpp libHellweg2D.cpp)

set_target_properties(Hellweg2D PROPERTIES 
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
    VERSION ${LIB_HW_VERSION_STRING}
    SOVERSION ${LIB_HW_VERSION_MAJOR}
)

install(TARGETS Hellweg2D
        DESTINATION lib)

install(FILES libHellweg2D.h
        DESTINATION include)

