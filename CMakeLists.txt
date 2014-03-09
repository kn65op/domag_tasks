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

#compiler options
set(CMAKE_CXX_FLAGS "-std=c++11")

#includes
include_directories(${SRC_DIR})

#files
file(GLOB EXAMPLE_FILES ${SRC_DIR}/*.cpp)
#file(GLOB GMOCK_TEST_FILES ${SRC_DIR}/GmockTest/*.cpp)

#executables
add_executable(${DEBUG_BIN_DIR}/${PROJECT_NAME} ${MAIN_FILE})

#test
add_subdirectory(Test)
