cmake_minimum_required (VERSION 3.2.3) 

#variables
set(SRC_DIR "Code")
set(MAIN_SRC_DIR "${SRC_DIR}/Main")
set(DEBUG_BIN_DIR "Debug")
set(RELEASE_BIN_DIR "Release")
set(LIBRARIES_DIR "libraries")
set(LIBRARIES_BIN_DIR "libraries")

#project
project (domag)

#external libs
FIND_PACKAGE( Boost 1.46 COMPONENTS date_time REQUIRED )

set(TLogger_DIR "${CMAKE_BINARY_DIR}/${LIBRARIES_BIN_DIR}/TLogger")
set(THelper_DIR "${CMAKE_BINARY_DIR}/${LIBRARIES_BIN_DIR}/THelper")
set(BUILD_TESTS "false")

add_subdirectory(${LIBRARIES_DIR}/TLogger)
FIND_PACKAGE( TLogger REQUIRED PATHS "${LIBRARIES_BIN_DIR}/TLogger" CONFIG)

add_subdirectory(${LIBRARIES_DIR}/THelper)
FIND_PACKAGE( THelper REQUIRED PATHS ${LIBRARIES}/THelper CONFIG)

set(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "No yaml build tools")
add_subdirectory(${LIBRARIES_DIR}/yaml-cpp)
FIND_PACKAGE( yaml-cpp  REQUIRED PATHS ${LIBRARIES}/yaml-cpp CONFIG)

include(ExternalProject)

ExternalProject_Add(cli
	            SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/${LIBRARIES_DIR}/cli-toolkit/build/make"
		    CONFIGURE_COMMAND ""
		    BUILD_IN_SOURCE 1
		    BUILD_COMMAND make cpp
		    INSTALL_COMMAND "")

#compiler options
set(CMAKE_CXX_FLAGS "-std=c++1z")
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

link_directories(${TLOGGER_LIBRARIES_DIR})

set(CLI_LIBRARY clicpp ncurses )
set(CLI_LIBRARY_DIR libraries/cli-toolkit/cpp/build/make/Linuxg++/Release)
link_directories(${CLI_LIBRARY_DIR})

set(LIBDOMAGCLI_INCLUDE_DIR Code/libdomagcli/include)

#includes
include_directories(${SRC_DIR})
include_directories(${Boost_INCLUDE_DIR})
include_directories(${TLOGGER_INCLUDE_DIRS})
include_directories(${THELPER_INCLUDE_DIRS})
include_directories(${YAML_CPP_INCLUDE_DIR})
include_directories(${LIBDOMAGCLI_INCLUDE_DIR})

add_subdirectory(${SRC_DIR})

#depot
file(GLOB DEPOT_FILES ${SRC_DIR}/depot/src/*.cpp)
include_directories(${SRC_DIR}/depot/inc/)

#main
file(GLOB MAIN_FILES ${SRC_DIR}/Main/src/*.cpp)
include_directories(${SRC_DIR}/Main/inc/)

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
add_executable(${EXE_FILE} ${MAIN_FILES}
		           ${DEPOT_FILES})

set(LIBDOMAGCLI_LIB domagcli)
#libraries
TARGET_LINK_LIBRARIES(${EXE_FILE}
                      ${Boost_LIBRARIES}
		      ${TLOGGER_LIBRARIES}
		      ${YAML_CPP_LIBRARIES}
		      ${LIBDOMAGCLI_LIB}
		      ${CLI_LIBRARY}
		      )

add_dependencies(${EXE_FILE} ${TLOGGER_TARGET})
			  
