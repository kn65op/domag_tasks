#pragma once

#include <memory>

#include "gtkmm.h"

namespace gui
{

class MainWindow
{
public:
  std::unique_ptr<Gtk::Window> getWindow();
  Gtk::TreeView* getContainersTreeView();

private:
  Glib::RefPtr<Gtk::Builder> builder;
};
}