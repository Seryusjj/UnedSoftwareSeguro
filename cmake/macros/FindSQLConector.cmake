#
# Find the MySQL Connector cpp 1.1.7
#

# This module defines
# MYSQL_CONNECTOR_INCLUDE_DIR, where to find mysql_connection.h
# MYSQL_CONNECTOR_LIBRARIES, the libraries to link against to connect to MySQL Connector
# MYSQL_CONNECTOR_FOUND, if false, you cannot build anything that requires MySQL Connector.

# also defined
# MYSQL_CONNECTOR_LIBRARY, where to find the MySQLConnector library.
# MYSQL_CONNECTOR_SHARED_LIBRARY, where is the shared lib related to MYSQL_CONNECTOR_LIBRARY

set(MYSQL_CONNECTOR_FOUND 0)


if( WIN32 )
	# read environment variables and change \ to /
	SET(PROGRAM_FILES_32 $ENV{ProgramFiles})
	if (${PROGRAM_FILES_32})
	STRING(REPLACE "\\\\" "/" PROGRAM_FILES_32 ${PROGRAM_FILES_32})
	endif(${PROGRAM_FILES_32})

	SET(PROGRAM_FILES_64 $ENV{ProgramW6432})
	if (${PROGRAM_FILES_64})
	 STRING(REPLACE "\\\\" "/" PROGRAM_FILES_64 ${PROGRAM_FILES_64})
	endif(${PROGRAM_FILES_64})

    #find include dir
	find_path(MYSQL_CONNECTOR_INCLUDE_DIR
	  NAMES
		mysql_connection.h
	  PATHS
		${MYSQL_CONNECTOR_ADD_INCLUDE_PATH}
		"${PROGRAM_FILES_64}/MySQL/MySQL Connector C++ 1.1.7/include"
		"${PROGRAM_FILES_32}/MySQL/MySQL Connector C++ 1.1.7/include"
	  DOC
		"Specify the directory containing mysql_connection.h."
	)
	
	#find lib dir
	find_library(MYSQL_CONNECTOR_LIBRARY
	NAMES
	  mysqlcppconn
	PATHS
	  ${MYSQL_CONNECTOR_ADD_LIBRARIES_PATH}
	   "${PROGRAM_FILES_64}/MySQL/MySQL Connector C++ 1.1.7/lib/opt"
	   "${PROGRAM_FILES_32}/MySQL/MySQL Connector C++ 1.1.7/lib/opt"
	DOC "Specify the location of the mysqlcppconn.lib library here."
	)
	
	
	STRING(REPLACE ".lib" ".dll" MYSQL_CONNECTOR_SHARED_LIBRARY ${MYSQL_CONNECTOR_LIBRARY})
	
endif( WIN32 )



if( MYSQL_CONNECTOR_LIBRARY )
  if( MYSQL_CONNECTOR_INCLUDE_DIR )
    set( MYSQL_CONNECTOR_FOUND 1 )
    message(STATUS "Found Connector library: ${MYSQL_CONNECTOR_LIBRARY}")
    message(STATUS "Found Connector headers: ${MYSQL_CONNECTOR_INCLUDE_DIR}")
  else( MYSQL_CONNECTOR_INCLUDE_DIR )
    message(FATAL_ERROR "Could not find Connector headers! Please install the development libraries and headers")
  endif( MYSQL_CONNECTOR_INCLUDE_DIR )
  mark_as_advanced( MYSQL_CONNECTOR_FOUND MYSQL_CONNECTOR_LIBRARY MYSQL_CONNECTOR_INCLUDE_DIR)
else( MYSQL_CONNECTOR_LIBRARY )
  message(FATAL_ERROR "Could not find the Connector connector libraries! Please install the development libraries and headers for version  1.1.7")
endif( MYSQL_CONNECTOR_LIBRARY )
