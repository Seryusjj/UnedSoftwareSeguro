#Server cmakelists
file(GLOB SRCS *.cpp *h)

add_executable(Server ${SRCS})

#include the src dir of the lib in order to keep a clean the code
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../common)
target_link_libraries(Server LibCommon)


#linq sqlconnector
include_directories(${MYSQL_CONNECTOR_INCLUDE_DIR})
target_link_libraries(Server ${MYSQL_CONNECTOR_LIBRARY})

		
#linq boost for mysqlcppconn, only need headers 
include_directories(${Boost_INCLUDE_DIR})


add_custom_command(TARGET Server POST_BUILD         # Adds a post-build event to Server
    COMMAND ${CMAKE_COMMAND} -E copy_if_different   # which executes "cmake - E copy_if_different..."
        ${MYSQL_CONNECTOR_SHARED_LIBRARY}	      	# <--this is in-file
        $<TARGET_FILE_DIR:Server>)                  # <--this is out-file path