#Server cmakelists
file(GLOB SRCS *.cpp *h)

add_executable(Server ${SRCS})

#include the src dir of the lib in order to keep a cleaner code
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../common)
target_link_libraries(Server LibCommon)


#linq sqlconnector
include_directories(${MYSQL_CONNECTOR_INCLUDE_DIR})
target_link_libraries(Server ${MYSQL_CONNECTOR_LIBRARY})

		
#linq boost for mysqlcppconn, only nedd headers 
include_directories(${Boost_INCLUDE_DIR})

