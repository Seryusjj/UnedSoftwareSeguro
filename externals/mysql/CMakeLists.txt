if (NOT MYSQL_FOUND)
  MESSAGE (FATAL_ERROR "Need to have installed mysqlserver from http://dev.mysql.com/downloads/mysql/")
endif()

add_library(mysql STATIC IMPORTED GLOBAL)

set_target_properties(mysql	PROPERTIES
	IMPORTED_LOCATION "${MYSQL_LIBRARY}"
	INTERFACE_INCLUDE_DIRECTORIES "${MYSQL_INCLUDE_DIR}"
)


