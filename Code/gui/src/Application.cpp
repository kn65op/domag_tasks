#include "gui/Application.hpp"

#include "gui/Builder.hpp"
#include "gui/ContainersTreeView.hpp"
#include "gui/NewContainerDialog.hpp"

#include "depot/inc/HomeContainerCatalog.hpp"
#include "depot/inc/AddNewContainerProcedure.hpp"

#include "gtkmm.h"

namespace gui
{

std::unique_ptr<Builder> builder;

void Application::prepareView()
{
    auto view = builder->getContainersTreeView();
    view->refresh();
}

void Application::openNewContainerDialog()
{
    const auto dialog = builder->getNewContainerDialog();
    dialog->setParentContainer();
    dialog->setProcedure(
        std::make_unique<depot::AddTopLevelContainerProcedure>(std::make_shared<depot::HomeContainerCatalog>()));
    builder->getContainersTreeView()->addTopLevelContainer(dialog->run());
}

Application::Application()
{
    auto app = Gtk::Application::create("org.domag");
    builder = std::make_unique<Builder>();
    prepareView();
    auto addContainerMenuItem = builder->getAddTopLevelContainerMenuItem();
    addContainerMenuItem->signal_activate().connect(sigc::mem_fun(*this, &Application::openNewContainerDialog));

    app->run(*builder->getMainWindow());
}
}
