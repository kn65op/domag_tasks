#include "gui/Application.hpp"

#include "depot/inc/Container.h"
#include "depot/inc/HomeContainerCatalog.h"
#include "gui/ContainerColumnModel.hpp"
#include "gui/MainWindow.hpp"

#include "gtkmm.h"

using depot::Container;

namespace gui
{

std::unique_ptr<MainWindow> mainWindow;
std::unique_ptr<Gtk::Window> mainWindowGtk;

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
    static ContainerColumnModel columns{*view};
    columns.clear();
    const auto& containers = catalog.getTopLevelContainers();
    addContainers(containers, columns);
}

void newContainer()
{
    depot::HomeContainerCatalog catalog;
    catalog.createTopLevelContainer();
    prepareView(mainWindow->getContainersTreeView());
}

std::unique_ptr<Gtk::Dialog> prepareDialog(MainWindow& window)
{
    auto dialog = window.getNewContainerDialog();
    static auto buttonOk = window.getNewContainerDialogButtonOk();
    static auto buttonCancel = window.getNewContainerDialogButtonCancel();
    buttonOk->signal_clicked().connect([&]() {
        newContainer();
        dialog->hide();
    });
    buttonCancel->signal_clicked().connect([&]() { dialog->hide(); });
    return dialog;
}

void Application::openNewContainerDialog()
{
    static const auto dialog = prepareDialog(*mainWindow);
    dialog->set_transient_for(*mainWindowGtk);
    dialog->run();
}

void Application::newContainer()
{
    depot::HomeContainerCatalog catalog;
    catalog.createTopLevelContainer();
    prepareView(mainWindow->getContainersTreeView());
}

Application::Application()
{
    mainWindow = std::make_unique<MainWindow>();
    auto app = Gtk::Application::create("org.domag");
    mainWindowGtk = mainWindow->getWindow();
    prepareView(mainWindow->getContainersTreeView());
    auto addContainerMenuItem = mainWindow->getAddTopLevelContainerMenuItem();
    addContainerMenuItem->signal_activate().connect(sigc::mem_fun(*this, &Application::openNewContainerDialog));

    app->run(*mainWindowGtk);
}
}
