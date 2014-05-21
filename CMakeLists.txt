cmake_minimum_required (VERSION 2.8) 

#variables
set(SRC_DIR "Code")
set(MAIN_SRC_DIR "${SRC_DIR}/Main/src")
set(MAIN_FILE "${MAIN_SRC_DIR}/Main.cpp")
set(DEBUG_BIN_DIR "Debug")
set(RELEASE_BIN_DIR "Release")

#project
project (domag)

#externial libs
FIND_PACKAGE( Boost 1.54 COMPONENTS date_time REQUIRED )

#compiler options
set(CMAKE_CXX_FLAGS "-std=c++1y")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -fprofile-arcs -ftest-coverage")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

#tests
set(COVERAGE_COMMAND "gcov")

set(CTEST_MEMORYCHECK_COMMAND "valgrind")
set(CTEST_MEMORYCHECK_COMMAND_OPTIONS "--trace-children=yes --quiet
--tool=memcheck --leak-check=yes --show-reachable=yes
--num-callers=100 --verbose --demangle=yes")

#includes
include_directories(${SRC_DIR})

#files
file(GLOB EXAMPLE_FILES ${SRC_DIR}/*.cpp)
file(GLOB DEPOT_FILES ${SRC_DIR}/depot/src/*.cpp)

#executables
if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	add_definitions(--coverage)
	set(EXE_FILE ${DEBUG_BIN_DIR}/${PROJECT_NAME})
elseif (${CMAKE_BUILD_TYPE} STREQUAL "Release")
        set(EXE_FILE ${RELEASE_BIN_DIR}/${PROJECT_NAME})
endif (${CMAKE_BUILD_TYPE} STREQUAL "Debug")

add_executable(${EXE_FILE} ${MAIN_FILE})

#libraries
TARGET_LINK_LIBRARIES(${EXE_FILE} ${Boost_LIBRARIES} )

#test
add_subdirectory(Test)
