#pragma once

#include <memory>

#include "gtkmm-3.0/gtkmm.h"

namespace gui
{
namespace widget
{
class NewContainerDialog;
class ContainersTreeView;
}

class Builder
{
  public:
    Builder();
    explicit Builder(Glib::RefPtr<Gtk::Builder>);

    std::unique_ptr<Gtk::Button> getNewContainerDialogButtonOk();
    std::unique_ptr<Gtk::Button> getNewContainerDialogButtonCancel();
    std::unique_ptr<Gtk::Label> getNewContainerDialogParentLabel();
    std::unique_ptr<Gtk::Entry> getNewContainerDialogNameEntry();
    std::unique_ptr<Gtk::Menu> getNewContainerPopupMenu();
    std::unique_ptr<Gtk::MenuItem> getNewContainerPopupMenuAddContainerItem();
    std::shared_ptr<Gtk::Window> getMainWindow();
    std::unique_ptr<Gtk::MenuItem> getAddTopLevelContainerMenuItem();
    std::shared_ptr<widget::NewContainerDialog> getNewContainerDialog();
    std::shared_ptr<widget::ContainersTreeView> getContainersTreeView();

  private:
    const Glib::RefPtr<Gtk::Builder> builder;
};
}
