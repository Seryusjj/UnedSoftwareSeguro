#server cmakelists
set (CMAKE_C_FLAGS "-Wall -g")

file(GLOB SRCS *.cpp *h)

add_executable(server ${SRCS})
#include the src dir of the lib in order to keep a cleaner code
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../common)
target_link_libraries(server libCommon)

#this allow the test project to use this as a kind of lib
SET_TARGET_PROPERTIES(server PROPERTIES
    ENABLE_EXPORTS TRUE)