#include "gui/NewContainerDialog.hpp"

#include "depot/inc/HomeContainerCatalog.h"

namespace gui
{
namespace widget
{

void newContainer()
{
    depot::HomeContainerCatalog catalog;
    catalog.createTopLevelContainer();
}

NewContainerDialog::NewContainerDialog(BaseObjectType* baseObject, Glib::RefPtr<Gtk::Builder>& builderIn)
    : Gtk::Dialog{baseObject}, builder{builderIn}, okButton{builder.getNewContainerDialogButtonOk()},
      cancelButton{builder.getNewContainerDialogButtonCancel()}
{
    okButton->signal_clicked().connect([&]() {
            newContainer();
            this->hide();
        });
    cancelButton->signal_clicked().connect([&]() { this->hide(); });
}
}
}
