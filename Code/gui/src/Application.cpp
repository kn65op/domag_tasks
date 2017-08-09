#include "gui/Application.hpp"

#include "gui/MainWindow.hpp"
#include "gui/NewContainerDialog.hpp"
#include "gui/ContainersTreeView.hpp"

#include "gtkmm.h"

namespace gui
{

std::unique_ptr<MainWindow> mainWindow;

void Application::prepareView()
{
    auto view = mainWindow->getContainersTreeView();
    view->refresh();
}

void Application::openNewContainerDialog()
{
    const auto dialog = mainWindow->getNewContainerDialog();
    dialog->setParentContainer();
    dialog->run();
    prepareView();
}

Application::Application()
{
    auto app = Gtk::Application::create("org.domag");
    mainWindow = std::make_unique<MainWindow>();
    prepareView();
    auto addContainerMenuItem = mainWindow->getAddTopLevelContainerMenuItem();
    addContainerMenuItem->signal_activate().connect(sigc::mem_fun(*this, &Application::openNewContainerDialog));

    app->run(*mainWindow->getWindow());
}
}
