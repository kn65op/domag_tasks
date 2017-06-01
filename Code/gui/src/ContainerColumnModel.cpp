#include "gui/ContainerColumnModel.hpp"

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

int ContainerColumnModel::addRow(const std::string& name)
{
    const int id = calculateId();
    auto rowIt = *treeStore->append();
    fillRow(rowIt, name);
    rows.emplace(id, rowIt);
    return id;
}

int ContainerColumnModel::addRow(int parentId, const std::string& name)
{
    const int id = calculateId();
    auto parentRow = rows[parentId];
    auto rowIt = *treeStore->append(parentRow->children());
    fillRow(rowIt, name);
    rows.emplace(id, rowIt);
    return id;
}

void ContainerColumnModel::fillRow(Gtk::TreeStore::iterator& rowIt, const std::string& name)
{
    auto row = *rowIt;
    row[modelName] = name;
}

int ContainerColumnModel::calculateId()
{
    return ++i;
}

void ContainerColumnModel::clear()
{
    treeStore->clear();
}

}
