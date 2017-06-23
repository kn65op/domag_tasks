#pragma once

#include "gtkmm-3.0/gtkmm.h"

namespace gui
{
namespace widget
{

class ContainersTreeView : public Gtk::TreeView
{
  public:
    ContainersTreeView(BaseObjectType*, Glib::RefPtr<Gtk::Builder>&);
};
}
}
