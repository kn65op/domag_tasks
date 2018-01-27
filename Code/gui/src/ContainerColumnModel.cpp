#include "gui/ContainerColumnModel.hpp"

#include "depot/inc/ItemsContainer.h"

#include "TLogger.h"

namespace gui
{

ContainerColumnModel::ContainerColumnModel(Gtk::TreeView& view) : tree{view}
{
    add(modelId);
    add(modelName);
    add(modelInside);
    treeStore = Gtk::TreeStore::create(*this);
    tree.set_model(treeStore);
    tree.append_column("Name", modelName);
}

int ContainerColumnModel::addRow(std::shared_ptr<depot::HierarchicalItemsContainer> container)
{
    const int id = calculateId();
    auto rowIt = *treeStore->append();
    fillRow(rowIt, id, container);
    fillInternalData(rowIt, id, container);
    return id;
}

int ContainerColumnModel::addRow(int parentId, std::shared_ptr<depot::HierarchicalItemsContainer> container)
{
    const int id = calculateId();
    auto parentRow = rows[parentId];
    auto rowIt = *treeStore->append(parentRow->children());
    fillRow(rowIt, id, container);
    fillInternalData(rowIt, id, container);
    return id;
}

void ContainerColumnModel::fillRow(Gtk::TreeStore::iterator& rowIt, int id,
                                   const std::shared_ptr<depot::HierarchicalItemsContainer> container)
{
    auto row = *rowIt;
    row[modelName] = container->getName();
    row[modelId] = id;
}

void ContainerColumnModel::fillInternalData(Gtk::TreeStore::iterator& rowIt, const int id,
                                            std::shared_ptr<depot::HierarchicalItemsContainer> container)
{
    rows.emplace(id, rowIt);
    containers[id] = container;
}

int ContainerColumnModel::calculateId()
{
    return ++i;
}

void ContainerColumnModel::clear()
{
    treeStore->clear();
    rows.clear();
    containers.clear();
}

std::string ContainerColumnModel::getName(const Gtk::TreeRow& row)
{
    return row.get_value<Glib::ustring>(modelName);
}

std::shared_ptr<depot::HierarchicalItemsContainer> ContainerColumnModel::getContainer(const Gtk::TreeRow& row)
{
    LOG << row.get_value<int>(modelId);
    try
    {
        return containers.at(row.get_value<int>(modelId));
    }
    catch(const std::exception&)
    {
        LOG << "Unable to get container";
    }
    return nullptr;
}
}
