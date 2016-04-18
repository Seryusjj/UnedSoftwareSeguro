#Tests cmakelists
file(GLOB SRCS *.cpp *h)
file(GLOB RES *.properties )

add_executable(Tests ${SRCS} ${RES})


#include google test
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../externals/googletest/include)
target_link_libraries(Tests gtest)


#include the src dir of the lib in order to keep a cleaner code
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../common)
target_link_libraries(Tests LibCommon)

add_custom_command(TARGET Tests POST_BUILD          # Adds a post-build event to Tests
    COMMAND ${CMAKE_COMMAND} -E copy_if_different   # which executes "cmake - E copy_if_different..."
        ${RES}   									# <--this is in-file
        $<TARGET_FILE_DIR:Tests>)                  # <--this is out-file path

#need to execute test from IDE since the execution path is going to be &{CMAKE_CURRENT_BINARY_DIR}
#instead of ${CMAKE_BINARY_DIR}/bin wath causes the program to not find the config file
file(COPY ${RES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})       