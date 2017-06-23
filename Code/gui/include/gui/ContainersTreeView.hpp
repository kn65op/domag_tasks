#pragma once

#include "gtkmm-3.0/gtkmm.h"

#include "gui/Builder.hpp"

namespace gui
{
namespace widget
{

class ContainersTreeView : public Gtk::TreeView
{
  public:
    ContainersTreeView(BaseObjectType*, Glib::RefPtr<Gtk::Builder>&);

    void handleButtonPressed();
 private:
    Builder builder;
};
}
}
