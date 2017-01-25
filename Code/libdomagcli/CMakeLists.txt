# Variables
set(CLI_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/libraries/cli-toolkit/cpp/include)
set(DOMAGCLI_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include/libdomagcli)
set(DOMAGCLI_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
set(GENERATED_CLI_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR})
set(GENERATED_CLI_FILE ${GENERATED_CLI_INCLUDE_DIR}/domagCli.h)
set(GENERATOR_CLI ${CMAKE_SOURCE_DIR}/libraries/cli-toolkit/tools/cli2cpp.py)
set(GENERATOR_SOURCE ${CMAKE_SOURCE_DIR}/Resources/cli/domag.xml)

#Code generation
add_custom_command(
	OUTPUT ${GENERATED_CLI_FILE}
	COMMAND python ${GENERATOR_CLI} ${GENERATOR_SOURCE} > ${GENERATED_CLI_FILE}
	DEPENDS ${GENERATOR_CLI} ${GENERATOR_SOURCE}
	)

# Includes 
include_directories(${GENERATED_CLI_INCLUDE_DIR})
include_directories(${DOMAGCLI_INCLUDE_DIR})
include_directories(${CLI_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/Code/depot/inc)

# Library
add_library(domagcli STATIC
	${DOMAGCLI_SOURCE_DIR}/CliStarter.cpp
	${DOMAGCLI_SOURCE_DIR}/CliCallback.cpp
	${GENERATED_CLI_FILE}
	)


