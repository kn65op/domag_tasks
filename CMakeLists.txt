cmake_minimum_required (VERSION 2.8) 

#variables
set(SRC_DIR "Code")
set(MAIN_SRC_DIR "${SRC_DIR}/Main")
set(DEBUG_BIN_DIR "Debug")
set(RELEASE_BIN_DIR "Release")

#project
project (domag)

#external libs
FIND_PACKAGE( Boost 1.46 COMPONENTS date_time REQUIRED )
FIND_PACKAGE( TLogger REQUIRED PATHS TLogger)

#compiler options
set(CMAKE_CXX_FLAGS "-std=c++1y")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -fprofile-arcs -ftest-coverage")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG="-fprofile-arcs -ftest-coverage")
 
if ("${NO_COVERAGE}" STREQUAL "true")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
else ("${NO_COVERAGE}" STREQUAL "true")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage")
endif ("${NO_COVERAGE}" STREQUAL "true")

set(NORMAL_WARNING_FLAGS "${CMAKE_CXX_FLAGS_DEBUG}")
set(EXTRA_WARNING_FLAGS "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra")

set(CMAKE_CXX_FLAGS_DEBUG "${EXTRA_WARNING_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

#tests
set(COVERAGE_COMMAND "gcov")

set(CTEST_MEMORYCHECK_COMMAND "valgrind")
set(CTEST_MEMORYCHECK_COMMAND_OPTIONS "--trace-children=yes --quiet
--tool=memcheck --leak-check=yes --show-reachable=yes
--num-callers=100 --verbose --demangle=yes")

link_directories(${CMAKE_BINARY_DIR}/${TLOGGER_LIBRARIES_DIR})

#includes
include_directories(${SRC_DIR})
include_directories(${TLOGGER_INCLUDE_DIRS})

set(BUILD_TESTS "false")
add_subdirectory(TLogger)


#files
#file(GLOB EXAMPLE_FILES ${SRC_DIR}/*.cpp)
file(GLOB MAIN_FILES ${SRC_DIR}/Main/src/*.cpp)
file(GLOB DEPOT_FILES ${SRC_DIR}/depot/src/*.cpp)

#executables
if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	if ("${NO_COVERAGE}" STREQUAL "true")
	else ("${NO_COVERAGE}" STREQUAL "true")
		add_definitions(--coverage)
	endif ("${NO_COVERAGE}" STREQUAL "true")

	add_subdirectory(Test)
	set(EXE_FILE ${DEBUG_BIN_DIR}/${PROJECT_NAME})
elseif (${CMAKE_BUILD_TYPE} STREQUAL "Release")
else (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	message(FATAL_ERROR "Invalid build type. Set by -DCMAKE_BUILD_TYPE=Debug|Release")
endif (${CMAKE_BUILD_TYPE} STREQUAL "Debug")

set(EXE_FILE ${PROJECT_NAME})
add_executable(${EXE_FILE} ${MAIN_FILES})

#libraries
TARGET_LINK_LIBRARIES(${EXE_FILE} ${Boost_LIBRARIES}
                                  ${TLOGGER_LIBRARIES} )

add_dependencies(${EXE_FILE} ${TLOGGER_TARGET})
			  
