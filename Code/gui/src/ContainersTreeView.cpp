#include "gui/ContainersTreeView.hpp"

#include "TLogger.h"

#include "depot/inc/Container.h"
#include "depot/inc/HomeContainerCatalog.h"
#include "gui/NewContainerDialog.hpp"

namespace gui
{
namespace widget
{

void addContainers(const depot::Container::Containers& containers, ContainerColumnModel& model, int id)
{
    for (const auto& container : containers)
    {
        const auto new_id = model.addRow(id, container->getName());
        addContainers(container->getContainers(), model, new_id);
    }
}

void addContainers(const depot::Container::Containers& containers, ContainerColumnModel& model)
{
    for (const auto& container : containers)
    {
        const auto id = model.addRow(container->getName());
        addContainers(container->getContainers(), model, id);
    }
}

ContainersTreeView::ContainersTreeView(BaseObjectType* base, Glib::RefPtr<Gtk::Builder>& builderIn)
    : Gtk::TreeView{base}, builder{builderIn}, addNewContainerMenu{builder.getNewContainerPopupMenu()},
      addNewContainerMenuItem{builder.getNewContainerPopupMenuAddContainerItem()}
{
    signal_popup_menu().connect([&]() {
        openNewContainerDialogMenu();
        return true;
    });
    addNewContainerMenuItem->signal_activate().connect([&]() {
        const auto dialog = builder.getNewContainerDialog();
        LOG << this->get_selection()->count_selected_rows();
        const auto& selected = this->get_selection()->get_selected();
        const auto name = columns.getName(*selected);
        LOG << "Container name: " << name;
        dialog->setParentContainer(name);
        dialog->run();
        LOG << "Run finished";
    });
}

void ContainersTreeView::openNewContainerDialogMenu(const GdkEvent* event)
{
    const auto& selected = this->get_selection()->get_selected();
    if (selected)
    {
        addNewContainerMenu->popup_at_pointer(event);
    }
}

void ContainersTreeView::refresh()
{
    columns.clear();
    depot::HomeContainerCatalog catalog;
    addContainers(catalog.getTopLevelContainers(), columns);
}

bool ContainersTreeView::on_button_press_event(GdkEventButton* event)
{
    const auto result = Gtk::TreeView::on_button_press_event(event);
    if (event->type == GDK_BUTTON_PRESS && event->button == 3)
    {
        openNewContainerDialogMenu(reinterpret_cast<GdkEvent*>(event));
    }
    return result;
}
}
}
