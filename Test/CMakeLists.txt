set(TEST_DIR ".")
set(TEST_EXE_DIR "${TEST_DIR}/Exe")
set(TEST_SRC_DIR "${TEST_DIR}/src")
set(TEST_INC_DIR "${TEST_DIR}/inc")
set(TEST_EXE ${TEST_EXE_DIR}/Tester)

set(GMOCK_DIR /usr/src/gmock)
set(GTEST_DIR /usr/src/gtest)
set(GMOCK_SOURCE_DIR ${GMOCK_DIR}/src)
set(GTEST_SOURCE_DIR ${GTEST_DIR}/src)

set(LIB_GMOCK gmock)

#projects parts
set(TESTS_POSTFIX Tests)
set(MOCKS_POSTFIX Mocks)

set(DEPOT_TESTS_DIR depot${TESTS_POSTFIX})
set(DEPOT_MOCKS_DIR depot${MOCKS_POSTFIX})

#project includes
include_directories(${PROJECT_SOURCE_DIR}/${SRC2_DIR}/GmockTest)
include_directories(${TEST_INC_DIR}/${DEPOT_MOCKS_DIR}/)

#externial libs

include_directories(${GTEST_DIR})
add_subdirectory(${GMOCK_DIR} gmock)

#files
file(GLOB MAIN_TEST_FILES ${TEST_SRC_DIR}/*.cpp)

file(GLOB DEPOT_TEST_FILES ${TEST_SRC_DIR}/${DEPOT_TESTS_DIR}/*.cpp)

#tests
enable_testing()

file(MAKE_DIRECTORY ${TEST_EXE_DIR})

add_executable(${TEST_EXE} ${MAIN_TEST_FILES} ${DEPOT_TEST_FILES})
target_link_libraries(${TEST_EXE} ${LIB_GMOCK})
add_test(gtests ${TEST_EXE}) 
