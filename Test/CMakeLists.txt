set(TEST_DIR ".")
set(TEST_EXE_DIR "${TEST_DIR}")
set(TEST_SRC_DIR "${TEST_DIR}/src")
set(TEST_INC_DIR "${TEST_DIR}/inc")
set(TEST_EXE Tester)

if ("${GMOCK_DIR}" STREQUAL "")
	message(STATUS "No GMOCK_DIR variable set, trying to use system gmock")
	set(GMOCK_DIR "/usr/src")
	set(GMOCK_H_INCLUDE_DIR /usr/include/gmock)
	set(GMOCK_MAIN_DIR /usr/src/gmock)
else ("${GMOCK_DIR}" STREQUAL "")
	set(GMOCK_MAIN_DIR "${GMOCK_DIR}")
	set(GMOCK_INCLUDE_DIR "${GMOCK_MAIN_DIR}/include")
	set(GMOCK_H_INCLUDE_DIR "${GMOCK_DIR}/include/gmock")
	set(GTEST_INCLUDE_DIR "${GMOCK_MAIN_DIR}/gtest/include")
	include_directories(${GMOCK_INCLUDE_DIR})
	include_directories(${GTEST_INCLUDE_DIR})
endif ("${GMOCK_DIR}" STREQUAL "")

set(GMOCK_INCLUDE "${GMOCK_H_INCLUDE_DIR}/gmock.h")
set(LIB_GMOCK gmock)

if (EXISTS "${GMOCK_INCLUDE}")
	message(STATUS "Using gmock from ${GMOCK_MAIN_DIR}")
else (EXISTS "${GMOCK_INCLUDE}")
	message(FATAL_ERROR "Could not find googlemock in ${GMOCK_DIR}")
endif (EXISTS "${GMOCK_INCLUDE}")

set(GMOCK_INCLUDE_DIR "${GMOCK_DIR}/include/gmock")
set(GMOCK_INCLUDE "${GMOCK_INCLUDE_DIR}/gmock.h")

#projects parts
set(TESTS_POSTFIX Tests)
set(MOCKS_POSTFIX Mocks)

set(DEPOT_TESTS_DIR depot${TESTS_POSTFIX})
set(DEPOT_MOCKS_DIR depot${MOCKS_POSTFIX})

#project includes
include_directories(${TEST_INC_DIR}/${DEPOT_MOCKS_DIR}/)

#externial libs

set(CMAKE_CXX_FLAGS_DEBUG "${NORMAL_WARNING_FLAGS}")
add_subdirectory(${GMOCK_MAIN_DIR} gmock)
set(CMAKE_CXX_FLAGS_DEBUG "${EXTRA_WARNING_FLAGS}")

#test files
file(GLOB MAIN_TEST_FILES ${TEST_SRC_DIR}/*.cpp)

file(GLOB DEPOT_TEST_FILES ${TEST_SRC_DIR}/${DEPOT_TESTS_DIR}/*.cpp)

#tests
enable_testing()

file(MAKE_DIRECTORY ${TEST_EXE_DIR})

add_executable(${TEST_EXE} ${MAIN_TEST_FILES} ${DEPOT_FILES} ${DEPOT_TEST_FILES})
TARGET_LINK_LIBRARIES(${TEST_EXE} ${Boost_LIBRARIES} )
target_link_libraries(${TEST_EXE} ${LIB_GMOCK})
TARGET_LINK_LIBRARIES(${TEST_EXE} ${TLOGGER_LIBRARIES})
add_test(NAME gtests COMMAND ${TEST_EXE}) 

add_dependencies(${TEST_EXE} ${TLOGGER_TARGET})

set(COVERAGE_DIR coverage)
set(SOURCE_RAW_FILES_DIR "${CMAKE_BINARY_DIR}/Test/CMakeFiles/Tester.dir/__/Code")
set(COVERAGE_INFO domag.info)

add_custom_target(coverage)
add_custom_command(TARGET coverage
	COMMAND mkdir -p ${COVERAGE_DIR}
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_custom_command(TARGET coverage
	COMMAND find ${CMAKE_BINARY_DIR} -name *.gcda -exec rm {} "\;" && ../${TEST_EXE}  && lcov -d  ${SOURCE_RAW_FILES_DIR} --capture --output-file ${COVERAGE_INFO} && genhtml ${COVERAGE_INFO}
	WORKING_DIRECTORY ${COVERAGE_DIR})

add_dependencies(coverage ${TEST_EXE})


add_custom_target(coverage_view)
add_custom_command(TARGET coverage_view
	COMMAND x-www-browser index.html
	WORKING_DIRECTORY ${COVERAGE_DIR})

add_dependencies(coverage_view coverage)
