#include "gui/Application.hpp"
#include "gui/MainWindow.hpp"

#include "gtkmm/application.h"

namespace gui
{

Application::Application()
{
  auto app = Gtk::Application::create("org.domag");
  auto window = MainWindow{}.getWindow();
  app->run(window);
}
}