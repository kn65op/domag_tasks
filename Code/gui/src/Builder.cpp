#include "gui/Builder.hpp"

namespace gui
{

Builder::Builder(Glib::RefPtr<Gtk::Builder> builderIn)
    : builder{std::move(builderIn)}
{
    
}

std::unique_ptr<Gtk::Button> Builder::getNewContainerDialogButtonOk()
{
    Gtk::Widget* addNewContainerDialog;
    builder->get_widget("Add container dialog button add", addNewContainerDialog);
    auto dialog = dynamic_cast<Gtk::Button*>(addNewContainerDialog);
    if (dialog)
    {
        return std::unique_ptr<Gtk::Button>{dialog};
    }
    else
    {
        throw 8;
    }
}

std::unique_ptr<Gtk::Button> Builder::getNewContainerDialogButtonCancel() 
{
    Gtk::Widget* addNewContainerDialog;
    builder->get_widget("Add container dialog button cancel", addNewContainerDialog);
    auto dialog = dynamic_cast<Gtk::Button*>(addNewContainerDialog);
    if (dialog)
    {
        return std::unique_ptr<Gtk::Button>{dialog};
    }
    else
    {
        throw 9;
    }
}

std::unique_ptr<Gtk::Entry> Builder::getNewContainerDialogNameEntry()
{
    Gtk::Widget* nameEntry;
    builder->get_widget("Add container dialog name entry", nameEntry);
    auto entry = dynamic_cast<Gtk::Entry*>(nameEntry);
    if (entry)
    {
        return std::unique_ptr<Gtk::Entry>{entry};
    }
    else
    {
        throw 9;
    }
}

}
