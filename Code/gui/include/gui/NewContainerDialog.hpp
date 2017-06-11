#pragma once

#include <memory>

#include "gtkmm-3.0/gtkmm.h"

#include "gui/Builder.hpp"

namespace gui
{
namespace widget
{
class NewContainerDialog : public Gtk::Dialog
{
public:
    NewContainerDialog(BaseObjectType*, Glib::RefPtr<Gtk::Builder>&);

private:
    void cleanAndHide();

    Builder builder;
    std::unique_ptr<Gtk::Button> okButton;
    std::unique_ptr<Gtk::Button> cancelButton;
    std::unique_ptr<Gtk::Entry> nameEntry;
};
}
}
