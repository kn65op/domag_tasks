#include "gui/ContainersTreeView.hpp"

#include "TLogger.h"

#include "depot/inc/AddNewContainerProcedure.hpp"
#include "depot/inc/Container.hpp"
#include "depot/inc/HomeContainerCatalog.hpp"
#include "depot/inc/RemoveContainerProcedure.hpp"
#include "gui/NewContainerDialog.hpp"

namespace gui
{
namespace widget
{

void addContainers(depot::Container::Containers& containers, ContainerColumnModel& model, int id)
{
    for (auto& container : containers)
    {
        const auto new_id = model.addRow(id, container);
        auto containers_inside = container->getContainers();
        addContainers(containers_inside, model, new_id);
    }
}

void addContainers(depot::Container::Containers& containers, ContainerColumnModel& model)
{
    for (auto& container : containers)
    {
        const auto id = model.addRow(container);
        auto containers_inside = container->getContainers();
        addContainers(containers_inside, model, id);
    }
}

ContainersTreeView::ContainersTreeView(BaseObjectType* base, Glib::RefPtr<Gtk::Builder>& builderIn)
    : Gtk::TreeView{base}, builder{builderIn}, addNewContainerMenu{builder.getContainerPopupMenu()},
      addNewContainerMenuItem{builder.getContainerPopupMenuAddContainerItem()},
      removeContainerMenuItem{builder.getContainerPopupMenuRemoveContainerItem()}
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
        dialog->setProcedure(std::make_unique<depot::AddDependentContainerProcedure>(columns.getContainer(*selected)));
        constexpr auto expand = true;
        LOG << "Update view";
        columns.addRow(columns.getId(*selected), dialog->run().lock(), expand);
        LOG << "Run finished";
    });
    removeContainerMenuItem->signal_activate().connect([&]() {
        const auto& selected = this->get_selection()->get_selected();
        const auto name = columns.getName(*selected);
        LOG << "Removing container: " << name;
        auto catalog = std::make_shared<depot::HomeContainerCatalog>();
        auto procedure = depot::RemoveAnyContainerProcedure(catalog);
        procedure.removeContainer(columns.getContainer(*selected));
        LOG << "Update view";
        columns.removeRow(columns.getId(*selected));
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
    auto containers = catalog.getTopLevelContainers();
    addContainers(containers, columns);
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

void ContainersTreeView::addTopLevelContainer(std::weak_ptr<depot::HierarchicalItemsContainer> container)
{
    columns.addRow(container.lock());
}

} // namespace widget
} // namespace gui
