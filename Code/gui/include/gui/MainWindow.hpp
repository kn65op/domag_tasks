#pragma once

#include <memory>

#include "gtkmm/window.h"

namespace gui
{

class MainWindow
{
public:
  std::unique_ptr<Gtk::Window> getWindow();
};
}