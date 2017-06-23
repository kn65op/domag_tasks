#include "gui/ContainersTreeView.hpp"

namespace gui
{
namespace widget
{
ContainersTreeView::ContainersTreeView(BaseObjectType* base, Glib::RefPtr<Gtk::Builder>& builderIn)
    : Gtk::TreeView{base}, builder{builderIn}
{
}
}
}
