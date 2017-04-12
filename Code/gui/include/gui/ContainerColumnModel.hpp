#pragma once

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

    Gtk::TreeModelColumn<int> modelId;
    Gtk::TreeModelColumn<Glib::ustring> modelName;
    Gtk::TreeModelColumn<Glib::ustring> modelInside;

    void addRow(const std::string & name)
    {
        static int i = 1;
        auto row = *treeStore->append();
        row[modelId] = i++;
        row[modelName] = name;
    }

private:
    Gtk::TreeView & tree;
    Glib::RefPtr< Gtk::TreeStore > treeStore;
};

}
