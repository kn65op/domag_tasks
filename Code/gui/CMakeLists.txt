link_directories(${GTKMM_LIBRARY_DIRS}) 

include_directories(${GTKMM_INCLUDE_DIRS})
include_directories(include)
include_directories(${DEPOT_INCLUDE})

set(GUI_SOURCE_DIR src)

set(RESOURCE_HEADER_FILE_NAME_IN_BUILD GladeMainWindow.hpp)
set(RESOURCE_FILE_IN_BUILD ${CMAKE_CURRENT_BINARY_DIR}/${RESOURCE_HEADER_FILE_NAME_IN_BUILD})
set(RESOURCE_FILE_WITH_NAME_IN_BUILD ${CMAKE_CURRNET_BINARY_DIR})#/${RESOURCE_FILE_NAME_IN_BUILD})
set(RESOURCE_FILE_IN_SOURCE ${CMAKE_CURRENT_LIST_DIR}/resources/domagMainWindow.glade)
set(RESOURCE_FILE_GENERATOR ${CMAKE_CURRENT_LIST_DIR}/generators/gladeToStringGenerator.sh)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_custom_command(
	OUTPUT ${RESOURCE_FILE_IN_BUILD}
	COMMAND ${RESOURCE_FILE_GENERATOR} ${RESOURCE_FILE_IN_SOURCE} ${RESOURCE_HEADER_FILE_NAME_IN_BUILD} 
	DEPENDS ${RESOURCE_FILE_IN_SOURCE} ${RESOURCE_FILE_GENERATOR}
	COMMENT "Translate glade files to source files")

add_library(gui 
    ${GUI_SOURCE_DIR}/Application.cpp
    ${GUI_SOURCE_DIR}/MainWindow.cpp
    ${GUI_SOURCE_DIR}/ContainerColumnModel.cpp
    ${GUI_SOURCE_DIR}/NewContainerDialog.cpp
    ${GUI_SOURCE_DIR}/Builder.cpp
    ${GUI_SOURCE_DIR}/ContainersTreeView.cpp
    ${RESOURCE_FILE_IN_BUILD}
    )

target_link_libraries(gui 
	    ${GTKMM_LIBRARIES}
	    )
