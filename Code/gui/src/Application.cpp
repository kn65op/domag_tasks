#include "gui/Application.hpp"
#include "gui/MainWindow.hpp"

#include "gtkmm.h"

namespace gui
{

class ModelColumns : public Gtk::TreeModelColumnRecord
{
public:
  ModelColumns()
  {
    add(m_col_text);
    add(m_col_number);
  }

  Gtk::TreeModelColumn<Glib::ustring> m_col_text;
  Gtk::TreeModelColumn<int> m_col_number;
};

void prepareView(Gtk::TreeView* view)
{
  ModelColumns m_Columns;
  auto refListStore = Gtk::TreeStore::create(m_Columns);
  view->set_model(refListStore);
  view->append_column("Messages", m_Columns.m_col_text);
  view->append_column("id", m_Columns.m_col_number);
  auto iter = refListStore->append();
  auto row = *iter;
  row[m_Columns.m_col_number] = 1;
  row[m_Columns.m_col_text] = "ASDSA";
  auto iter2 = refListStore->append();
  auto row2 = *iter2;
  row2[m_Columns.m_col_number] = 2;
  row2[m_Columns.m_col_text] = "QQQ";

  auto iter_child = refListStore->append(row.children());
  auto row_child = *iter_child;
  row_child[m_Columns.m_col_number] = 4;
  row_child[m_Columns.m_col_text] = "ZZZ";
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