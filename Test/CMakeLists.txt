set(TEST_DIR ".")
set(TEST_EXE_DIR "${TEST_DIR}/Exe")
set(TEST_EXE ${TEST_EXE_DIR}/Tester)

set(GMOCK_DIR /usr/src/gmock)
set(GTEST_DIR /usr/src/gtest)
set(GMOCK_SOURCE_DIR ${GMOCK_DIR}/src)
set(GTEST_SOURCE_DIR ${GTEST_DIR}/src)

set(LIB_GMOCK gmock)

#project includes
include_directories(${PROJECT_SOURCE_DIR}/${SRC2_DIR}/GmockTest)

#externial libs

include_directories(${GTEST_DIR})
add_subdirectory(${GMOCK_DIR} gmock)

#files
file(GLOB TEST_FILES ${TEST_DIR}/*.cpp)

#tests
enable_testing()


add_executable(${TEST_EXE} ${TEST_FILES} ${GMOCK_TEST_FILES})
target_link_libraries(${TEST_EXE} ${LIB_GMOCK})
add_test(gtests ${TEST_EXE}) 
