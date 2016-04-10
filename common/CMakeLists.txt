#LibCommon cmakelists
file(GLOB HEADERS *.h)
file(GLOB SRCS *.cpp *h)
#this lib is not really use never (temporally needed)
add_library(LibCommonsh SHARED ${SRCS})
add_library(LibCommon STATIC ${SRCS})

