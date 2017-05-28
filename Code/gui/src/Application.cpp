#include "gui/Application.hpp"
#include "depot/inc/Container.h"
#include "depot/inc/HomeContainerCatalog.h"
#include "gui/ContainerColumnModel.hpp"
#include "gui/MainWindow.hpp"

#include "gtkmm.h"

using depot::Container;

namespace gui
{

void addContainers(const Container::Containers& containers, ContainerColumnModel& model, int id)
{
    for (const auto& container : containers)
    {
        const auto new_id = model.addRow(id, container->getName());
        addContainers(container->getContainers(), model, new_id);
    }
}
void addContainers(const Container::Containers& containers, ContainerColumnModel& model)
{
    for (const auto& container : containers)
    {
        const auto id = model.addRow(container->getName());
        addContainers(container->getContainers(), model, id);
    }
}

void prepareView(Gtk::TreeView* view)
{
    depot::HomeContainerCatalog catalog;
    ContainerColumnModel columns{*view};
    const auto& containers = catalog.getTopLevelContainers();
    addContainers(containers, columns);
    // const auto idToAddChild = m_Columns.addRow("");
    // m_Columns.addRow("QQ");

    // const auto idToSecondChild = m_Columns.addRow(idToAddChild, "Child one");
    // m_Columns.addRow(idToSecondChild, "Child in child");
    // m_Columns.addRow(idToAddChild, "Child two");
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
