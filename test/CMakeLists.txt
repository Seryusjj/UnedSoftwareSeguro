#tests cmakelists
file(GLOB SRCS *.cpp *h)

add_executable(TESTS ${SRCS})



#include libCommon
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../common)
target_link_libraries(TESTS libCommon)


#include google test
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../externalLibs/googletest/include)
target_link_libraries(TESTS gtest)

               