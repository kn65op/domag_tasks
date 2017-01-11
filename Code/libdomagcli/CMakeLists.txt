set(DOMAGCLI_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
set(DOMAGCLI_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include/libdomagcli)

set(GENERATED_CLI_INCLUDE_DIR ${CMAKE_HOME_DIRECTORY}/Debug)
set(CLI_INCLUDE_DIR ${CMAKE_HOME_DIRECTORY}/libraries/cli-toolkit/cpp/include)

include_directories(${GENERATED_CLI_INCLUDE_DIR})
include_directories(${DOMAGCLI_INCLUDE_DIR})
include_directories(${CLI_INCLUDE_DIR})

add_library(domagcli STATIC
	${DOMAGCLI_SOURCE_DIR}/CliStarter.cpp)


