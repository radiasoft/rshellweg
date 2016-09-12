set(Catch_INCLUDE_DIRS ../../../deps/Catch/single_include)
set(libHellweg2D_INCLUDE_DIRS ..)

set(TEST_SRC testHellweg2D.cpp testAnsiString.cpp testTIniFile.cpp)

include_directories(${libHellweg2D_INCLUDE_DIRS})
include_directories(${Catch_INCLUDE_DIRS})

add_executable (Hellweg2DTest ${TEST_SRC})
target_link_libraries(Hellweg2DTest Hellweg2D)
