#include "gui/NewContainerDialog.hpp"

#include <iostream>

#include "TLogger.h"

#include "gui/ContainersTreeView.hpp"

#include "depot/inc/HomeContainerCatalog.h"

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
        newContainer(nameEntry->get_buffer()->get_text());
        cleanAndHide();
    });
    cancelButton->signal_clicked().connect([&]() { cleanAndHide(); });
}

void NewContainerDialog::setParentContainer(const std::string& parent)
{
    parentLabel->set_text(parent);
}

void NewContainerDialog::cleanAndHide()
{
    hide();
    auto buffer = nameEntry->get_buffer();
    buffer->set_text({});
}

void NewContainerDialog::run()
{
    LOG << "RUN";
    Gtk::Dialog::run();
    builder.getContainersTreeView()->refresh();
}

}
}
