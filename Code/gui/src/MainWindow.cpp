#include "gui/MainWindow.hpp"

#include "GladeMainWindow.hpp"

#include <iostream>

#include "gtkmm.h"

namespace gui
{
std::unique_ptr<Gtk::Window> MainWindow::getWindow()
{
    try
    {
        builder = Gtk::Builder::create_from_string(gladeXml);
        Gtk::Widget* windowP;
        builder->get_widget("main_window", windowP);
        return std::unique_ptr<Gtk::Window>{dynamic_cast<Gtk::Window*>(windowP)};
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
    return nullptr;
}

Gtk::TreeView* MainWindow::getContainersTreeView()
{
    static Gtk::Widget* treeView{};
    if (treeView)
    {
        auto tree = dynamic_cast<Gtk::TreeView*>(treeView);
        return tree;
    }
    else
    {
        builder->get_widget("container_tree_view", treeView);
        auto tree = dynamic_cast<Gtk::TreeView*>(treeView);
        if (tree)
        {
            return tree;
        }
        else
        {
            throw 1;
        }
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

std::unique_ptr<Gtk::Dialog> MainWindow::getNewContainerDialog()
{
    Gtk::Widget* addNewContainerDialog;
    builder->get_widget("Add container dialog", addNewContainerDialog);
    auto dialog = dynamic_cast<Gtk::Dialog*>(addNewContainerDialog);
    if (dialog)
    {
        return std::unique_ptr<Gtk::Dialog>{dialog};
    }
    else
    {throw 7;}
}

}
