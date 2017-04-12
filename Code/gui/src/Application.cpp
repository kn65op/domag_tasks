#include "gui/Application.hpp"
#include "gui/ContainerColumnModel.hpp"
#include "gui/MainWindow.hpp"

#include "gtkmm.h"

namespace gui
{

void prepareView(Gtk::TreeView* view)
{
    ContainerColumnModel m_Columns{*view};
    const auto idToAddChild = m_Columns.addRow("ASD");
    m_Columns.addRow("QQ");

    const auto idToSecondChild = m_Columns.addRow(idToAddChild, "Child one");
    m_Columns.addRow(idToSecondChild, "Child in child");
    m_Columns.addRow(idToAddChild, "Child two");
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
