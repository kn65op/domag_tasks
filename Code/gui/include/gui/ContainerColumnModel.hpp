#pragma once

#include <functional>
#include <map>

#include "gtkmm.h"
namespace depot
{
class HierarchicalItemsContainer;
}

namespace gui
{

class ContainerColumnModel : public Gtk::TreeModelColumnRecord
{
public:
    ContainerColumnModel(Gtk::TreeView& view);

    int addRow(std::shared_ptr<depot::HierarchicalItemsContainer>);
    int addRow(int parentId, std::shared_ptr<depot::HierarchicalItemsContainer>);
    int addRow(int parentId, std::shared_ptr<depot::HierarchicalItemsContainer>, bool);
    void removeRow(int id);
    void clear();
    std::string getName(const Gtk::TreeRow&) const;
    int getId(const Gtk::TreeRow&) const;
    std::shared_ptr<depot::HierarchicalItemsContainer> getContainer(const Gtk::TreeRow&);

private:
    int i = 0;
    Gtk::TreeView& tree;
    Glib::RefPtr<Gtk::TreeStore> treeStore;
    Gtk::TreeModelColumn<int> modelId;
    Gtk::TreeModelColumn<Glib::ustring> modelInside;
    Gtk::TreeModelColumn<Glib::ustring> modelName;
    std::map<int, Gtk::TreeStore::iterator> rows;
    std::map<int, std::shared_ptr<depot::HierarchicalItemsContainer>> containers;

    void fillRow(Gtk::TreeStore::iterator& rowIt, int, const std::shared_ptr<depot::HierarchicalItemsContainer>);
    void fillInternalData(Gtk::TreeStore::iterator&, int, std::shared_ptr<depot::HierarchicalItemsContainer>);
    int calculateId();
};
} // namespace gui
