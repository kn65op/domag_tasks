#pragma once

#include "gtkmm.h"

namespace gui
{

class ContainerColumnModel : public Gtk::TreeModelColumnRecord
{
public:
    ContainerColumnModel()
    {
        add(m_col_text);
        add(m_col_number);
    }

    Gtk::TreeModelColumn<Glib::ustring> m_col_text;
    Gtk::TreeModelColumn<int> m_col_number;
};

}
