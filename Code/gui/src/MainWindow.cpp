#include "gui/MainWindow.hpp"

#include <iostream>

#include "gtkmm.h"

namespace gui
{
std::unique_ptr<Gtk::Window> MainWindow::getWindow()
{
  try
  {
    auto builder = Gtk::Builder::create_from_file("MainWindow.glade");
    Gtk::Widget *windowP;
    builder->get_widget("main_window", windowP);
    return std::unique_ptr<Gtk::Window>{dynamic_cast<Gtk::Window *>(windowP)};
  }
  catch (const Glib::FileError &ex)
  {
    std::cerr << "FileError: " << ex.what() << std::endl;
  }
  catch (const Glib::MarkupError &ex)
  {
    std::cerr << "MarkupError: " << ex.what() << std::endl;
  }
  catch (const Gtk::BuilderError &ex)
  {
    std::cerr << "BuilderError: " << ex.what() << std::endl;
  }
  return nullptr;
}
}