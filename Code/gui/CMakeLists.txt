link_directories(${GTKMM_LIBRARY_DIRS}) 

include_directories(${GTKMM_INCLUDE_DIRS})
include_directories(include)

set(GUI_SOURCE_DIR src)


add_library(gui 
    ${GUI_SOURCE_DIR}/Application.cpp
    ${GUI_SOURCE_DIR}/MainWindow.cpp
    )

target_link_libraries(gui 
	    ${GTKMM_LIBRARIES})
