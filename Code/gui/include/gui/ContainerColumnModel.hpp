#pragma once

#include "gtkmm.h"

namespace gui
{

class ContainerColumnModel : public Gtk::TreeModelColumnRecord
{
public:
    ContainerColumnModel(Gtk::TreeView & view) :
        tree{view}
    {
        add(id);
        add(name);
        add(inside);
    }

    Gtk::TreeModelColumn<int> id;
    Gtk::TreeModelColumn<Glib::ustring> name;
    Gtk::TreeModelColumn<Glib::ustring> inside;

private:
    Gtk::TreeView & tree;
};

}
