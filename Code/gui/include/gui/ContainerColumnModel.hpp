#pragma once

#include "gtkmm.h"

namespace gui
{

class ContainerColumnModel : public Gtk::TreeModelColumnRecord
{
public:
    ContainerColumnModel(Gtk::TreeView& view) : tree{view}
    {
        add(id);
        add(name);
        add(inside);
        treeStore = Gtk::TreeStore::create(*this);
    }

    Gtk::TreeModelColumn<int> id;
    Gtk::TreeModelColumn<Glib::ustring> name;
    Gtk::TreeModelColumn<Glib::ustring> inside;
    Glib::RefPtr< Gtk::TreeStore > treeStore;

private:
    Gtk::TreeView & tree;
};

}
