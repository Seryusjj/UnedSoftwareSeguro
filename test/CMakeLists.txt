#Tests cmakelists
file(GLOB SRCS *.cpp *h)

add_executable(Tests ${SRCS})

#include LibCommon
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../common)
target_link_libraries(Tests LibCommon)

#include google test
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../externals/googletest/include)
target_link_libraries(Tests gtest)


               