#Server cmakelists
file(GLOB SRCS *.cpp *h)
file(GLOB RES *.properties )

add_executable(Server ${SRCS} ${RES})

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
        ${MYSQL_CONNECTOR_SHARED_LIBRARY} ${RES}   	# <--this is in-file
        $<TARGET_FILE_DIR:Server>)                  # <--this is out-file path
		
#need to execute test from IDE since the execution path is going to be &{CMAKE_CURRENT_BINARY_DIR}
#instead of ${CMAKE_BINARY_DIR}/bin wath causes the program to not find the config file
file(COPY ${RES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}) 		