cmake_minimum_required (VERSION 2.8) 

#variables
set(PROJECT_NAME "domag")
set(SRC_DIR "Src")
set(MAIN_SRC_DIR "Src/Main")
set(MAIN_FILE "${MAIN_SRC_DIR}/Main.cpp")
set(DEBUG_BIN_DIR "Debug")
set(RELEASE_BIN_DIR "Release")

#project
project (DOMAG)

#externial libs

#compiler options
set(CMAKE_CXX_FLAGS "-std=c++11")


#files
file(GLOB EXAMPLE_FILES ${SRC_DIR}/*.cpp)

file(GLOB TEST_FILES ${TEST_DIR}/*.cpp)

#executables
add_executable(${DEBUG_BIN_DIR}/exe_debug ${EXAMPLE_FILES} ${MAIN_FILE})

#test
add_subdirectory(Test)
