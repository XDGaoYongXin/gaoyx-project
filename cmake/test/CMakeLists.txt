PROJECT (test_gaoyx)
cmake_minimum_required(VERSION 2.6)

AUX_SOURCE_DIRECTORY(../../test SRC_TEST)

ADD_EXECUTABLE(test_gaoyx ${SRC_TEST})
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../../bin)

if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
     ADD_DEFINITIONS(-D_DEBUG)
else()
     ADD_DEFINITIONS(-DNDEBUG)
endif()

INCLUDE_DIRECTORIES("../../thread"
                    "../../network")

LINK_DIRECTORIES("/usr/local/lib")

add_dependencies(${PROJECT_NAME} network thread)

target_link_libraries(${PROJECT_NAME} network thread pthread "/usr/local/lib/libevent.a")
