if (NOT MYSQL_CONNECTOR_FOUND)
  MESSAGE (FATAL_ERROR "Need to have installed mysql connector 1.1.7 from https://dev.mysql.com/downloads/connector/cpp/1.1.html")
endif()


add_library(connector STATIC IMPORTED GLOBAL)

set_target_properties(connector PROPERTIES
	IMPORTED_LOCATION "${MYSQL_CONNECTOR_LIBRARY}"
	INTERFACE_INCLUDE_DIRECTORIES "${MYSQL_CONNECTOR_INCLUDE_DIR}"
)


