#include "gui/MainWindow.hpp"

#include "GladeMainWindow.hpp"

#include "gui/ContainersTreeView.hpp"
#include "gui/NewContainerDialog.hpp"

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
    static std::unique_ptr<Gtk::Widget> treeView{};
    if (treeView)
    {
        auto tree = dynamic_cast<Gtk::TreeView*>(treeView.get());
        return tree;
    }
    else
    {
        widget::ContainersTreeView* t;
        builder->get_widget_derived("container_tree_view", t);
        treeView.reset(t);
        auto tree = dynamic_cast<Gtk::TreeView*>(treeView.get());
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

std::unique_ptr<widget::NewContainerDialog> MainWindow::getNewContainerDialog()
{
    widget::NewContainerDialog* addNewContainerDialog{nullptr};
    builder->get_widget_derived("Add container dialog", addNewContainerDialog);
    if (addNewContainerDialog)
    {
        return std::unique_ptr<widget::NewContainerDialog>{addNewContainerDialog};
    }
    else
    {
        throw 7;
    }
}

std::unique_ptr<Gtk::Button> MainWindow::getNewContainerDialogButtonOk()
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

std::unique_ptr<Gtk::Button> MainWindow::getNewContainerDialogButtonCancel()
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

std::unique_ptr<Gtk::Menu> MainWindow::getNewContainerPopupMenu()
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
