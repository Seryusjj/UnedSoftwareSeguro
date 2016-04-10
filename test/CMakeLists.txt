#Tests cmakelists
file(GLOB SRCS *.cpp *h)

add_executable(Tests ${SRCS})


#include google test
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../externals/googletest/include)
target_link_libraries(Tests gtest)


#include the src dir of the lib in order to keep a cleaner code
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../common)
target_link_libraries(Tests LibCommon)

               