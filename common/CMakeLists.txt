#common lib cmakelists, make the lib static in order to avoid 
#a dll distribution
SET (CMAKE_C_FLAGS "-Wall -g")

FILE(GLOB SRCS *.cpp *h)
ADD_LIBRARY(libCommon STATIC ${SRCS})