cmake_minimum_required(VERSION 2.8)

project (libHellweg2D)

set(LIB_HW_VERSION_MAJOR 0)
set(LIB_HW_VERSION_MINOR 1)
set(LIB_HW_VERSION_PATCH 0)
set(LIB_HW_VERSION_STRING ${LIB_HW_VERSION_MAJOR}.${LIB_HW_VERSION_MINOR}.${LIB_HW_VERSION_PATCH})

set(PHYS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../physics)
set(PHYS_SRC ${PHYS_DIR}/BeamSolver.cpp ${PHYS_DIR}/Spline.cpp ${PHYS_DIR}/Matrix.cpp 
             ${PHYS_DIR}/Beam.cpp ${PHYS_DIR}/Spectrum.cpp ${PHYS_DIR}/ConstUnit.cpp)

set(BUILDER_SRC AnsiString.cpp TStringList.cpp IniFiles.cpp)

add_definitions(-DRSLINAC)

find_package(Boost)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${PHYS_DIR})


add_library(Hellweg2D SHARED  
            ${PHYS_SRC}
            ${BUILDER_SRC}
            libHellweg2D.cpp)

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

enable_testing()
add_subdirectory (test)
add_test (NAME Hellweg2D COMMAND Hellweg2DTest)
