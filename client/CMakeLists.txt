#client cmakelists
file(GLOB SRCS *.cpp *h)

add_executable(Client ${SRCS})
#include the src dir of the lib in order to keep a cleaner code
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../common)
target_link_libraries(Client LibCommon)
