#common lib cmakelists, make the lib static in order to avoid 
#a dll distribution
FILE(GLOB SRCS *.cpp *h)
ADD_LIBRARY(libCommon STATIC ${SRCS})