if (NOT Boost_FOUND)
  MESSAGE (FATAL_ERROR "Need to boost 1.6.0 installed
endif()

add_library(boost STATIC IMPORTED GLOBAL)

set_target_properties(boost	PROPERTIES
	IMPORTED_LOCATION "${Boost_LIBRARY_DIRS}"
	INTERFACE_INCLUDE_DIRECTORIES "${Boost_INCLUDE_DIRS}"
)