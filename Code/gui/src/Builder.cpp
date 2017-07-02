#include "gui/Builder.hpp"

namespace gui
{

Builder::Builder(Glib::RefPtr<Gtk::Builder> builderIn) : builder{std::move(builderIn)}
{
}

template <typename Widget>
std::unique_ptr<Widget> tryGetWidget(const Glib::RefPtr<Gtk::Builder> builder, const std::string& widget_name)
{
    Gtk::Widget* widget;
    builder->get_widget(widget_name, widget);
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
    return tryGetWidget<Gtk::Button>(builder, "Add container dialog button add");
}

std::unique_ptr<Gtk::Button> Builder::getNewContainerDialogButtonCancel()
{
    return tryGetWidget<Gtk::Button>(builder, "Add container dialog button cancel");
}

std::unique_ptr<Gtk::Label> Builder::getNewContainerDialogParentLabel()
{
    return tryGetWidget<Gtk::Label>(builder, "Add container dialog parent label");
}

std::unique_ptr<Gtk::Entry> Builder::getNewContainerDialogNameEntry()
{
    return tryGetWidget<Gtk::Entry>(builder, "Add container dialog name entry");
}

std::unique_ptr<Gtk::Menu> Builder::getNewContainerPopupMenu()
{
    return tryGetWidget<Gtk::Menu>(builder, "Add container popup menu");
}

std::unique_ptr<Gtk::MenuItem> Builder::getNewContainerPopupMenuAddContainerItem()
{
    return tryGetWidget<Gtk::MenuItem>(builder, "Add container popup menu Add container button");
}
}
