#pragma once

#include "gtkmm-3.0/gtkmm.h"

#include "gui/Builder.hpp"
#include "gui/ContainerColumnModel.hpp"

namespace gui
{
namespace widget
{

class ContainersTreeView : public Gtk::TreeView
{
  public:
    ContainersTreeView(BaseObjectType*, Glib::RefPtr<Gtk::Builder>&);

    void refresh();

  private:
    Builder builder;
    ContainerColumnModel columns{*this};
    std::unique_ptr<Gtk::Menu> addNewContainerMenu;
    std::unique_ptr<Gtk::MenuItem> addNewContainerMenuItem;

    bool on_button_press_event(GdkEventButton *) override;
    void openNewContainerDialogMenu(const GdkEvent* event = nullptr);
};
}
}
