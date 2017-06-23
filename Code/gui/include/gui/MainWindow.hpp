#pragma once

#include <memory>

#include "gtkmm.h"

namespace gui
{

namespace widget
{
class NewContainerDialog;
class ContainersTreeView;
}

class MainWindow
{
  public:
    std::unique_ptr<Gtk::Window> getWindow();
    widget::ContainersTreeView* getContainersTreeView();
    Gtk::MenuItem* getAddTopLevelContainerMenuItem();
    std::unique_ptr<widget::NewContainerDialog> getNewContainerDialog();
    std::unique_ptr<Gtk::Button> getNewContainerDialogButtonOk();
    std::unique_ptr<Gtk::Button> getNewContainerDialogButtonCancel();

  private:
    Glib::RefPtr<Gtk::Builder> builder;
};
}
