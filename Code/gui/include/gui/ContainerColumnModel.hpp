#pragma once

#include <map>

#include "gtkmm.h"

namespace gui
{

class ContainerColumnModel : public Gtk::TreeModelColumnRecord
{
  public:
    ContainerColumnModel(Gtk::TreeView& view) : tree{view}
    {
        add(modelId);
        add(modelName);
        add(modelInside);
        treeStore = Gtk::TreeStore::create(*this);
        tree.set_model(treeStore);
        tree.append_column("id", modelId);
        tree.append_column("Messages", modelName);
    }

    int addRow(const std::string& name)
    {
        const int id = ++i;
        auto rowIt = *treeStore->append();
        auto row = *rowIt;
        fillRow(row, id, name);
        rows.emplace(id, rowIt);
        return id;
    }

    int addRow(int parentId, const std::string& name)
    {
        auto parentRow = rows[parentId];
        const int id = ++i;
        auto rowIt = *treeStore->append(parentRow->children());
        auto row = *rowIt;
        fillRow(row, id, name);
        rows.emplace(id, rowIt);
        return id;
    }

    void fillRow(Gtk::TreeRow &row, int id, const std::string &name)
    {
        row[modelId] = id;
        row[modelName] = name;
    }

  private:
    int i = 0;
    Gtk::TreeView& tree;
    Glib::RefPtr<Gtk::TreeStore> treeStore;
    Gtk::TreeModelColumn<int> modelId;
    Gtk::TreeModelColumn<Glib::ustring> modelName;
    Gtk::TreeModelColumn<Glib::ustring> modelInside;
    std::map<int, Gtk::TreeStore::iterator> rows;
};
}
