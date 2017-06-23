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
    signal_button_press_event().connect_notify(
        [&](GdkEventButton* event) {
            openNewContainerDialogMenu(reinterpret_cast<GdkEvent*>(event));
            std::cout << "buuton\n";
        },
        false);
    signal_popup_menu().connect([&]() {
        openNewContainerDialogMenu();
        std::cout << "popup \n";
        return true;
    });
}

void ContainersTreeView::openNewContainerDialogMenu(GdkEvent* event)
{
    const auto& selected = this->get_selection()->get_selected();
    if (selected)
    {
        std::cout << (*selected)[columns.modelName] << "\n";
        addNewContainerMenu->popup_at_pointer(event);
    }
}

void ContainersTreeView::refresh()
{
    columns.clear();
    depot::HomeContainerCatalog catalog;
    addContainers(catalog.getTopLevelContainers(), columns);
}
}
}
