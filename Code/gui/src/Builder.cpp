#include "gui/Builder.hpp"

namespace gui
{

Builder::Builder(Glib::RefPtr<Gtk::Builder> builderIn) : builder{std::move(builderIn)}
{
}

template <typename Widget>
std::unique_ptr<Widget> getWidget(Gtk::Builder builder, const std::string &widget_name)
{
    GtkWidget* widget;
    builder.get_widget(widget_name, widget);
    auto concreteWidget = dynamic_cast<Widget*>(widget);
    if (concreteWidget)
    {
        return std::unique_ptr<Widget>{concreteWidget};
    }
    else
    {
        throw "No valid widget";
    }
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

std::unique_ptr<Gtk::Menu> Builder::getNewContainerPopupMenu()
{
    Gtk::Widget* addNewContainerMenu;
    builder->get_widget("Add container popup menu", addNewContainerMenu);
    auto menu = dynamic_cast<Gtk::Menu*>(addNewContainerMenu);
    if (menu)
    {
        return std::unique_ptr<Gtk::Menu>{menu};
    }
    else
    {
        throw 9;
    }
}

}
