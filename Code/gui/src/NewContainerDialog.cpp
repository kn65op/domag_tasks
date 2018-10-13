#include "gui/NewContainerDialog.hpp"

#include <iostream>

#include "TLogger.h"

#include "gui/ContainersTreeView.hpp"

#include "depot/inc/AddNewContainerProcedure.hpp"
#include "depot/inc/ItemsContainer.hpp"
#include "depot/inc/HomeContainerCatalog.hpp"

namespace gui
{
namespace widget
{

void newContainer(const std::string& name)
{
    depot::HomeContainerCatalog catalog;
    catalog.createTopLevelContainer(name);
}

NewContainerDialog::NewContainerDialog(BaseObjectType* baseObject, Glib::RefPtr<Gtk::Builder>& builderIn)
    : Gtk::Dialog{baseObject}, builder{builderIn}, okButton{builder.getNewContainerDialogButtonOk()},
      cancelButton{builder.getNewContainerDialogButtonCancel()}, nameEntry{builder.getNewContainerDialogNameEntry()},
      parentLabel{builder.getNewContainerDialogParentLabel()}
{
    okButton->signal_clicked().connect([&]() {
        if (procedure)
        {
            const auto name = nameEntry->get_buffer()->get_text();
            newContainer = procedure->add(name);
        }
        else
        {
            LOG << "Missing procedure, no add";
        }
        cleanAndHide();
    });
    cancelButton->signal_clicked().connect([&]() { cleanAndHide(); });
}

void NewContainerDialog::setParentContainer(const std::string& parent)
{
    parentLabel->set_text(parent);
}

void NewContainerDialog::setProcedure(std::unique_ptr<depot::AddContainerProcedure> p)
{
    procedure = std::move(p);
}

void NewContainerDialog::cleanAndHide()
{
    hide();
    auto buffer = nameEntry->get_buffer();
    buffer->set_text({});
}

std::weak_ptr<depot::HierarchicalItemsContainer> NewContainerDialog::run()
{
    LOG << "RUN";
    Gtk::Dialog::run();
    return newContainer;
}
}
}
