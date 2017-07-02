cmake_minimum_required (VERSION 3.2.3) 

#global config
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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
FIND_PACKAGE( Boost 1.60 COMPONENTS date_time REQUIRED )
find_package(PkgConfig)

pkg_check_modules(GTKMM gtkmm-3.0)

set(TLogger_DIR "${CMAKE_BINARY_DIR}/${LIBRARIES_BIN_DIR}/TLogger")
set(THelper_DIR "${CMAKE_BINARY_DIR}/${LIBRARIES_BIN_DIR}/THelper")

set(NO_TEST "true")
add_subdirectory(${LIBRARIES_DIR}/TLogger)
FIND_PACKAGE( TLogger REQUIRED PATHS "${LIBRARIES_BIN_DIR}/TLogger" CONFIG)

add_subdirectory(${LIBRARIES_DIR}/THelper)
FIND_PACKAGE( THelper REQUIRED PATHS ${LIBRARIES}/THelper CONFIG)
set(NO_TEST "false")

#YAML-cpp
set(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "No yaml build tools")
add_subdirectory(${LIBRARIES_DIR}/yaml-cpp)
set(YAML_CPP_LIBRARIES yaml-cpp)
set(YAML_CPP_INCLUDE_DIR ${LIBRARIES_DIR}/yaml-cpp/include)

#compiler options
set(CMAKE_CXX_FLAGS "-std=c++1z")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -fprofile-arcs -ftest-coverage")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG="-fprofile-arcs -ftest-coverage")
 
if ("${NO_COVERAGE}" STREQUAL "true")
	SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
else ("${NO_COVERAGE}" STREQUAL "true")
        SET(GCC_COVERAGE_COMPILE_FLAGS "-fprofile-arcs -ftest-coverage")
        SET(GCC_COVERAGE_LINK_FLAGS    "-lgcov")
        SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
        SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")
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

#global includes
include_directories(${Boost_INCLUDE_DIR})

include_directories(${TLOGGER_INCLUDE_DIRS})
include_directories(${THELPER_INCLUDE_DIRS})

include_directories(${YAML_CPP_INCLUDE_DIR})
include_directories(${SRC_DIR})
include_directories(${SRC_DIR}/gui/include)

#depot
file(GLOB DEPOT_FILES ${SRC_DIR}/depot/src/*.cpp)
set(DEPOT_INCLUDE ${SRC_DIR}/depot/inc/)
include_directories(${DEPOT_INCLUDE})

#gui
add_subdirectory(${SRC_DIR})

#main
file(GLOB MAIN_FILES ${SRC_DIR}/Main/src/*.cpp)
include_directories(${SRC_DIR}/Main/inc/)

#executables

add_subdirectory(Test)
set(EXE_FILE ${PROJECT_NAME})

set(EXE_FILE ${PROJECT_NAME})
add_executable(${EXE_FILE} ${MAIN_FILES}
		           ${DEPOT_FILES})

#libraries
TARGET_LINK_LIBRARIES(${EXE_FILE}
                      ${Boost_LIBRARIES}
		      ${TLOGGER_LIBRARIES}
		      ${YAML_CPP_LIBRARIES}
		      gui)

add_dependencies(${EXE_FILE} ${TLOGGER_TARGET})

