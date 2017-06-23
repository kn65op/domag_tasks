#pragma once

#include <map>

#include "gtkmm.h"

namespace gui
{

class ContainerColumnModel : public Gtk::TreeModelColumnRecord
{
  public:
    ContainerColumnModel(Gtk::TreeView& view);

    int addRow(const std::string& name);
    int addRow(int parentId, const std::string& name);
    void clear();

    Gtk::TreeModelColumn<Glib::ustring> modelName;
  private:
    int i = 0;
    Gtk::TreeView& tree;
    Glib::RefPtr<Gtk::TreeStore> treeStore;
    Gtk::TreeModelColumn<int> modelId;
    //Gtk::TreeModelColumn<Glib::ustring> modelName;
    Gtk::TreeModelColumn<Glib::ustring> modelInside;
    std::map<int, Gtk::TreeStore::iterator> rows;

    void fillRow(Gtk::TreeStore::iterator& rowIt, const std::string& name);
    int calculateId();
};
}
