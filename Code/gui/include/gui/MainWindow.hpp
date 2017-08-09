#pragma once

#include <memory>

#include "gtkmm.h"

namespace gui
{
class Builder;

namespace widget
{
class NewContainerDialog;
class ContainersTreeView;
}

class MainWindow
{
  public:
    MainWindow();

    Gtk::Window* getWindow();
    widget::ContainersTreeView* getContainersTreeView();
    Gtk::MenuItem* getAddTopLevelContainerMenuItem();
    widget::NewContainerDialog* getNewContainerDialog();
    std::unique_ptr<Builder> createBuilder() const;

  private:
    Glib::RefPtr<Gtk::Builder> builder;
    std::unique_ptr<Gtk::Window> window;
};
}
