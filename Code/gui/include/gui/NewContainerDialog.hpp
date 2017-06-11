#pragma once

#include "gtkmm-3.0/gtkmm.h"

namespace gui
{
namespace widget
{
class NewContainerDialog : public Gtk::Dialog
{
  public:
    NewContainerDialog(BaseObjectType*, Glib::RefPtr<Gtk::Builder>&);
};
}
}
