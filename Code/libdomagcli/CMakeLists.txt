set(DOMAGCLI_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)

add_library(domagcli STATIC
	${DOMAGCLI_SOURCE_DIR}/CliStarter.cpp)


