#include "gui/Application.hpp"
#include "gui/MainWindow.hpp"
#include "gui/ContainerColumnModel.hpp"

#include "gtkmm.h"

namespace gui
{

void prepareView(Gtk::TreeView* view)
{
  ContainerColumnModel m_Columns{*view};
  auto refListStore = Gtk::TreeStore::create(m_Columns);
  view->set_model(refListStore);
  view->append_column("Messages", m_Columns.name);
  view->append_column("id", m_Columns.id);
  auto iter = refListStore->append();
  auto row = *iter;
  row[m_Columns.id] = 1;
  row[m_Columns.name] = "ASDSA";
  auto iter2 = refListStore->append();
  auto row2 = *iter2;
  row2[m_Columns.id] = 2;
  row2[m_Columns.name] = "QQQ";

  auto iter_child = refListStore->append(row.children());
  auto row_child = *iter_child;
  row_child[m_Columns.id] = 4;
  row_child[m_Columns.name] = "ZZZ";
}

Application::Application()
{
  auto app = Gtk::Application::create("org.domag");
  MainWindow mainWindow;
  auto window = mainWindow.getWindow();
  prepareView(mainWindow.getContainersTreeView());
  app->run(*window);
}
}
