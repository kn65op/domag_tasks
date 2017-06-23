#include "gui/Application.hpp"

#include "depot/inc/Container.h"
#include "depot/inc/HomeContainerCatalog.h"
#include "gui/ContainerColumnModel.hpp"
#include "gui/MainWindow.hpp"
#include "gui/NewContainerDialog.hpp"

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

void Application::prepareView()
{
    auto view = mainWindow->getContainersTreeView();
    static ContainerColumnModel columns{*view};
    auto openMenu = [&](const GdkEvent* event = nullptr) {
        static auto menu = mainWindow->getNewContainerPopupMenu();
        menu->popup_at_pointer(event);
    };
    view->set_activate_on_single_click(true);
    view->signal_row_activated().connect([&](const Gtk::TreeModel::Path&, Gtk::TreeViewColumn*) {
        std::cout << "activated\n";
    });
    view->get_selection()->signal_changed().connect([&]() {
        std::cout << "Selection\n";
    });
    view->signal_button_press_event().connect_notify(
        [&, view](GdkEventButton* event) {
            const auto& selected = view->get_selection()->get_selected();
            if (selected)
            {
                std::cout << (*selected)[columns.modelName] << "\n";
            }
            openMenu(reinterpret_cast<GdkEvent*>(event));
            std::cout << "buuton\n";
        },
        false);
    view->signal_popup_menu().connect([&, view]() {
        view->get_selection();
        openMenu();
        return true;
    });
    depot::HomeContainerCatalog catalog;
    columns.clear();
    const auto& containers = catalog.getTopLevelContainers();
    addContainers(containers, columns);
}

std::unique_ptr<widget::NewContainerDialog> prepareDialog(MainWindow& window)
{
    return window.getNewContainerDialog();
}

void Application::openNewContainerDialog()
{
    static const auto dialog = prepareDialog(*mainWindow);
    dialog->set_transient_for(*mainWindowGtk);
    dialog->run();
    prepareView();
}

Application::Application()
{
    mainWindow = std::make_unique<MainWindow>();
    auto app = Gtk::Application::create("org.domag");
    mainWindowGtk = mainWindow->getWindow();
    prepareView();
    auto addContainerMenuItem = mainWindow->getAddTopLevelContainerMenuItem();
    addContainerMenuItem->signal_activate().connect(sigc::mem_fun(*this, &Application::openNewContainerDialog));

    app->run(*mainWindowGtk);
}
}
