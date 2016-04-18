cmake_minimum_required(VERSION 2.4)

project(SecureClientServer)

#finds
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/macros)
find_package(Threads REQUIRED)

#find_package(MySQL)
find_package(SQLConector)
find_package(Boost 1.60.0 REQUIRED)

#if(COMMAND cmake_policy)
#  cmake_policy(SET CMP0002 NEW)
#endif(COMMAND cmake_policy)


#global confiurations, 
#set all orjects runtime binaries in the same path to avoid dealing with dll
SET (CMAKE_C_FLAGS "-Wall -g")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(BUILD_STATIC_LIBS OFF CACHE BOOL "Set to ON to build static libraries" FORCE)
set(BUILD_SHARED_LIBS ON CACHE BOOL "Set to ON to build DSO libraries" FORCE)
#add directories

add_subdirectory(externals)
add_subdirectory(common)
add_subdirectory(client)
add_subdirectory(server)
add_subdirectory(test)

#required for .dll or .so compilation
if (NOT BUILD_SHARED_LIBS)
	MESSAGE (FATAL_ERROR "Enable BUILD_SHARED_LIBS Required, I'll do it for u, try configure again")
endif()



