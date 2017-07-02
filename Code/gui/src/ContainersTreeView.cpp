#include "gui/ContainersTreeView.hpp"

#include "depot/inc/Container.h"
#include "depot/inc/HomeContainerCatalog.h"

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
    : Gtk::TreeView{base}, builder{builderIn}, addNewContainerMenu{builder.getNewContainerPopupMenu()}
{
    signal_popup_menu().connect([&]() {
        openNewContainerDialogMenu();
        return true;
    });
    //addNewContainerMenu->signal_activate().connect([]() { std::cout << "ASDA\n"; });
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
