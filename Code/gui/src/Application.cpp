#include "gui/Application.hpp"
#include "gui/MainWindow.hpp"
#include "gui/ContainerColumnModel.hpp"

#include "gtkmm.h"

namespace gui
{

void prepareView(Gtk::TreeView* view)
{
    ContainerColumnModel m_Columns{*view};
    m_Columns.addRow("ASD");
    m_Columns.addRow("QQ");

    /*
    auto iter_child = refListStore->append(row.children());
    auto row_child = *iter_child;
    row_child[m_Columns.id] = 4;
    row_child[m_Columns.name] = "ZZZ";
    */
}

Application::Application()
{
    auto app = Gtk::Application::create("org.domag");
    MainWindow mainWindow;
    auto window = mainWindow.getWindow();
    prepareView(mainWindow.getContainersTreeView());
    app->run(*window);
}
}
