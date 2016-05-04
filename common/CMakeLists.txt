#LibCommon cmakelists
file(GLOB SRCS *.cpp *h)
#this lib is not really use never (temporally needed)
add_library(LibCommon SHARED ${SRCS} )
#add_library(LibCommon STATIC ${SRCS})
include(GenerateExportHeader)

GENERATE_EXPORT_HEADER( LibCommon
             BASE_NAME LibCommon
             EXPORT_MACRO_NAME LibCommon_EXPORT
             EXPORT_FILE_NAME LibCommon_Export.h
             STATIC_DEFINE LibCommon_BUILT_AS_STATIC
)

