set(TEST_DIR ".")
set(TEST_EXE_DIR "${TEST_DIR}/Exe")
set(TEST_EXE ${TEST_EXE_DIR}/Tester)

set(gmock_SOURCE_DIR /usr/src/gmock/src)
set(gtest_SOURCE_DIR /usr/src/gmock/src)
set(gtest_dir /usr/src/gtest)

#externial libs

add_subdirectory(/usr/src/gtest gtest)
#add_subdirectory(/usr/src/gmock gmock)

#files
file(GLOB TEST_FILES ${TEST_DIR}/*.cpp)

#tests
enable_testing()

#add_library(gmock
#	"${gtest_dir}/src/gtest-all.cc")
#
#add_library(gmock_main
#	"${gtest_dir}/src/gtest-all.cc"
#	"${gtest_dir}/src/gtest-main.cc")
#
#include_directories("${gmock_SOURCE_DIR}/include"
#	"${gmock_SOURCE_DIR}"
#	"${gtest_SOURCE_DIR}/include"
#	"${gtest_SOURCE_DIR}")
#

add_executable(${TEST_EXE} ${TEST_FILES} ${EXAMPLE_FILES})
target_link_libraries(${TEST_EXE} gtest)
add_test(gtests ${TEST_EXE_DIR}/Tester) 
