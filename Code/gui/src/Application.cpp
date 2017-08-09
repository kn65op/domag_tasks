#include "gui/Application.hpp"

#include "gui/MainWindow.hpp"
#include "gui/NewContainerDialog.hpp"
#include "gui/ContainersTreeView.hpp"

#include "gtkmm.h"

namespace gui
{

std::unique_ptr<MainWindow> mainWindow;
Gtk::Window* mainWindowGtk;

void Application::prepareView()
{
    auto view = mainWindow->getContainersTreeView();
    view->refresh();
}

widget::NewContainerDialog* prepareDialog(MainWindow& window)
{
    return window.getNewContainerDialog();
}

void Application::openNewContainerDialog()
{
    const auto dialog = prepareDialog(*mainWindow);
    //dialog->set_transient_for(*mainWindowGtk);
    dialog->setParentContainer();
    dialog->run();
    prepareView();
}

Application::Application()
{
    auto app = Gtk::Application::create("org.domag");
    mainWindow = std::make_unique<MainWindow>();
    mainWindowGtk = mainWindow->getWindow();
    prepareView();
    auto addContainerMenuItem = mainWindow->getAddTopLevelContainerMenuItem();
    addContainerMenuItem->signal_activate().connect(sigc::mem_fun(*this, &Application::openNewContainerDialog));

    app->run(*mainWindowGtk);
}
}
