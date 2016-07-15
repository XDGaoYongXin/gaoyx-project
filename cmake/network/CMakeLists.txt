PROJECT(network)
cmake_minimum_required(VERSION  2.6)

AUX_SOURCE_DIRECTORY(../../network SRC_NETWORK)

ADD_LIBRARY(network STATIC ${SRC_NETWORK})
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../../bin/lib)

if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
      ADD_DEFINITIONS(-D_DEBUG)
else()
      ADD_DEFINITIONS(-DNDEBUG)
endif()

INCLUDE_DIRECTORIES("../../thread"
                    "../../network")
