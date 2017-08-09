#include "gui/MainWindow.hpp"

#include "GladeMainWindow.hpp"

#include "gui/ContainersTreeView.hpp"
#include "gui/NewContainerDialog.hpp"
#include "gui/ContainersTreeView.hpp"
#include "gui/Builder.hpp"

#include <iostream>

#include "gtkmm.h"

namespace gui
{
MainWindow::MainWindow()
{
    try
    {
        builder = Gtk::Builder::create_from_string(gladeXml);
        Gtk::Widget* windowP;
        builder->get_widget("main_window", windowP);
        window.reset(dynamic_cast<Gtk::Window*>(windowP));
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
}

Gtk::Window* MainWindow::getWindow()
{
    return window.get();
}

widget::ContainersTreeView* MainWindow::getContainersTreeView()
{
    static std::unique_ptr<widget::ContainersTreeView> treeView{};
    if (treeView)
    {
        return treeView.get();
    }
    else
    {
        widget::ContainersTreeView* t;
        builder->get_widget_derived("container_tree_view", t);
        treeView.reset(t);
        return treeView.get();
    }
}

Gtk::MenuItem* MainWindow::getAddTopLevelContainerMenuItem()
{
    Gtk::Widget* addContainerMenuItem;
    builder->get_widget("Add top level container menu item", addContainerMenuItem);
    auto menuItem = dynamic_cast<Gtk::MenuItem*>(addContainerMenuItem);
    if (menuItem)
    {
        return menuItem;
    }
    else
    {
        throw 1;
    }
}

widget::NewContainerDialog* MainWindow::getNewContainerDialog()
{
    static std::unique_ptr<widget::NewContainerDialog> newContainerDialog;
    if (newContainerDialog)
    {
        return newContainerDialog.get();
    }
    widget::NewContainerDialog* addNewContainerDialog{nullptr};
    builder->get_widget_derived("Add container dialog", addNewContainerDialog);
    if (addNewContainerDialog)
    {
        newContainerDialog.reset(addNewContainerDialog);
        newContainerDialog->set_transient_for(*window);
        return getNewContainerDialog();
    }
    else
    {
        throw 7;
    }
}

std::unique_ptr<Builder> MainWindow::createBuilder() const
{
    return std::make_unique<Builder>(builder);
}

}
