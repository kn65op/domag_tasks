#include "gui/Builder.hpp"

#include "TLogger.h"

#include "GladeMainWindow.hpp"
#include "gui/ContainersTreeView.hpp"
#include "gui/NewContainerDialog.hpp"

namespace gui
{

Builder::Builder(Glib::RefPtr<Gtk::Builder> builderIn) : builder{std::move(builderIn)}
{
}

template <typename Widget, typename Ptr>
Ptr tryGetWidget(const Glib::RefPtr<Gtk::Builder> builder, const std::string& widget_name)
{
    Gtk::Widget* widget;
    builder->get_widget(widget_name, widget);
    auto concreteWidget = dynamic_cast<Widget*>(widget);
    if (concreteWidget)
    {
        return Ptr{concreteWidget};
    }
    else
    {
        LOG << "Not found widget: " << widget_name;
        throw "No valid widget";
    }
}

template <typename Widget>
std::unique_ptr<Widget> tryGetUniqueWidget(const Glib::RefPtr<Gtk::Builder> builder, const std::string& widget_name)
{
    return tryGetWidget<Widget, std::unique_ptr<Widget>>(builder, widget_name);
}

template <typename Widget>
std::shared_ptr<Widget> tryGetSharedWidget(const Glib::RefPtr<Gtk::Builder> builder, const std::string& widget_name)
{
    return tryGetWidget<Widget, std::shared_ptr<Widget>>(builder, widget_name);
}

Builder::Builder() try : builder {Gtk::Builder::create_from_string(gladeXml)}
{
}
catch (const Glib::FileError& ex)
{
    std::cerr << "FileError: " << ex.what() << std::endl;
}
catch (const Glib::MarkupError& ex)
{
    std::cerr << "MarkupError: " << ex.what() << std::endl;
}
catch (const Gtk::BuilderError& ex)
{
    std::cerr << "BuilderError: " << ex.what() << std::endl;
}

std::unique_ptr<Gtk::Button> Builder::getNewContainerDialogButtonOk()
{
    return tryGetUniqueWidget<Gtk::Button>(builder, "Add container dialog button add");
}

std::unique_ptr<Gtk::Button> Builder::getNewContainerDialogButtonCancel()
{
    return tryGetUniqueWidget<Gtk::Button>(builder, "Add container dialog button cancel");
}

std::unique_ptr<Gtk::Label> Builder::getNewContainerDialogParentLabel()
{
    return tryGetUniqueWidget<Gtk::Label>(builder, "Add container dialog parent label");
}

std::unique_ptr<Gtk::Entry> Builder::getNewContainerDialogNameEntry()
{
    return tryGetUniqueWidget<Gtk::Entry>(builder, "Add container dialog name entry");
}

std::unique_ptr<Gtk::Menu> Builder::getNewContainerPopupMenu()
{
    return tryGetUniqueWidget<Gtk::Menu>(builder, "Add container popup menu");
}

std::unique_ptr<Gtk::MenuItem> Builder::getNewContainerPopupMenuAddContainerItem()
{
    return tryGetUniqueWidget<Gtk::MenuItem>(builder, "Add container popup menu Add container button");
}

std::shared_ptr<Gtk::Window> Builder::getMainWindow()
{
    static auto mainWindow = tryGetSharedWidget<Gtk::Window>(builder, "main_window");
    return mainWindow;
}

std::unique_ptr<Gtk::MenuItem> Builder::getAddTopLevelContainerMenuItem()
{
    return tryGetUniqueWidget<Gtk::MenuItem>(builder, "Add top level container menu item");
}

std::shared_ptr<widget::NewContainerDialog> Builder::getNewContainerDialog()
{
    static std::shared_ptr<widget::NewContainerDialog> newContainerDialog;
    if (newContainerDialog)
    {
        return newContainerDialog;
    }
    widget::NewContainerDialog* addNewContainerDialog{nullptr};
    builder->get_widget_derived("Add container dialog", addNewContainerDialog);
    if (addNewContainerDialog)
    {
        LOG << "Initialize new container dialog";
        newContainerDialog.reset(addNewContainerDialog);
        newContainerDialog->set_transient_for(*getMainWindow());
        return getNewContainerDialog();
    }
    else
    {
        throw 7;
    }
}

std::shared_ptr<widget::ContainersTreeView> Builder::getContainersTreeView()
{
    static std::shared_ptr<widget::ContainersTreeView> treeView{};
    if (treeView)
    {
        return treeView;
    }
    else
    {
        widget::ContainersTreeView* t;
        builder->get_widget_derived("container_tree_view", t);
        treeView.reset(t);
        return treeView;
    }
}

}
