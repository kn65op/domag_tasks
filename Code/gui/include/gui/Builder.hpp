#pragma once

#include <memory>

#include "gtkmm-3.0/gtkmm.h"

namespace gui
{

class Builder
{
  public:
    explicit Builder(Glib::RefPtr<Gtk::Builder>);

    std::unique_ptr<Gtk::Button> getNewContainerDialogButtonOk();
    std::unique_ptr<Gtk::Button> getNewContainerDialogButtonCancel();
    std::unique_ptr<Gtk::Label> getNewContainerDialogParentLabel();
    std::unique_ptr<Gtk::Entry> getNewContainerDialogNameEntry();
    std::unique_ptr<Gtk::Menu> getNewContainerPopupMenu();
    std::unique_ptr<Gtk::MenuItem> getNewContainerPopupMenuAddContainerItem();

  private:
    const Glib::RefPtr<Gtk::Builder> builder;
};
}
