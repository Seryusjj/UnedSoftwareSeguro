cmake_minimum_required(VERSION 2.4)



project(secure_client_server)

#finds
find_package(Threads REQUIRED)

#global confiurations, 
#set all orjects runtime binaries in the same path to avoid dealing with dll

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#add directories

add_subdirectory(common)
add_subdirectory(client)
add_subdirectory(server)
add_subdirectory(externalLibs)
add_subdirectory(test)



#required for google test
if (NOT BUILD_SHARED_LIBS)
	MESSAGE (FATAL_ERROR "Enable BUILD_SHARED_LIBS")
endif()

