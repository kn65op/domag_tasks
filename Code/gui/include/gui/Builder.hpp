#pragma once

#include "gtkmm-3.0/gtkmm.h"

namespace gui
{

class Builder
{
public:
    Builder(Glib::RefPtr<Gtk::Builder>);

    std::unique_ptr<Gtk::Button> getNewContainerDialogButtonOk();
    std::unique_ptr<Gtk::Button> getNewContainerDialogButtonCancel();
private:
    Glib::RefPtr<Gtk::Builder> builder;
};

}
