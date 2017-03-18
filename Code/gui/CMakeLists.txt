link_directories(${GTKMM_LIBRARY_DIRS}) 

include_directories(${GTKMM_INCLUDE_DIRS})
include_directories(include)

set(GUI_SOURCE_DIR src)

add_library(gui 
    ${GUI_SOURCE_DIR}/Application.cpp
    ${GUI_SOURCE_DIR}/MainWindow.cpp
    )

target_link_libraries(gui 
	    ${GTKMM_LIBRARIES}
	    )

set(RESOURCE_FILE_IN_BUILD ${CMAKE_BINARY_DIR}/MainWindow.glade)
set(RESOURCE_FILE_IN_SOURCE ${CMAKE_CURRENT_LIST_DIR}/resources/domagMainWindow.glade)

add_custom_command(
	TARGET gui POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy ${RESOURCE_FILE_IN_SOURCE} ${RESOURCE_FILE_IN_BUILD}
	DEPENDS ${RESOURCE_FILE_IN_SOURCE}
	COMMENT "Copying glade files")
