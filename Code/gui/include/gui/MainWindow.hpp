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
    Gtk::MenuItem* getAddTopLevelContainerMenuItem();
    std::unique_ptr<Gtk::Dialog> getNewContainerDialog();

  private:
    Glib::RefPtr<Gtk::Builder> builder;
};
}
