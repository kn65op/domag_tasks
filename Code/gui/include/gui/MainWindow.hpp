#pragma once

#include <memory>

#include "gtkmm.h"

namespace gui
{

namespace widget
{
class NewContainerDialog;
}

class MainWindow
{
  public:
    std::unique_ptr<Gtk::Window> getWindow();
    Gtk::TreeView* getContainersTreeView();
    Gtk::MenuItem* getAddTopLevelContainerMenuItem();
    std::unique_ptr<widget::NewContainerDialog> getNewContainerDialog();
    std::unique_ptr<Gtk::Button> getNewContainerDialogButtonOk();
    std::unique_ptr<Gtk::Button> getNewContainerDialogButtonCancel();
    std::unique_ptr<Gtk::Menu> getNewContainerPopupMenu();

  private:
    Glib::RefPtr<Gtk::Builder> builder;
};
}
