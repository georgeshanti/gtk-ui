#include "GuiSettings.hpp"
#include <iostream>

AddWindow::AddWindow(std::vector<Gesture> ges, List *li):add_button("ADD"), cancel_button("CANCEL"), grid(), command(),
hbox(), vbox(Gtk::ORIENTATION_VERTICAL)
{
  l=li;
  set_title("ComboBox example");

  //Create the Tree model:
  //m_refTreeModel = Gtk::TreeStore::create(m_Columns);
  m_refTreeModel = Gtk::ListStore::create(m_Columns);
  m_Combo.set_model(m_refTreeModel);

  //Fill the ComboBox's Tree Model:
  int i=0;
  for(auto s=ges.begin() ; s!=ges.end() ; s++){
    g[i] = *s;
    Gtk::TreeModel::Row row = *(m_refTreeModel->append());
    row[m_Columns.m_col_id] = i;
    row[m_Columns.m_col_name] = (*s).name();
    row[m_Columns.m_col_extra] = " ";
    if(i==0)
      m_Combo.set_active(row);
    i++;
  }

  // m_Combo.pack_start(m_Columns.m_col_id);
  m_Combo.pack_start(m_Columns.m_col_name);

  //An example of adding a cell renderer manually,
  //instead of using pack_start(model_column)
  //so we have more control:
  m_Combo.set_cell_data_func(m_cell,
    sigc::mem_fun(*this, &AddWindow::on_cell_data_extra));
  m_Combo.pack_start(m_cell);

  //Add the ComboBox to the window.
  grid.set_column_homogeneous(TRUE);
  grid.set_row_spacing(10);
  grid.set_column_spacing(10);

  grid.attach(m_Combo, 0, 0, 2, 1);
  grid.attach(command, 0, 1, 2, 1);
  grid.attach(cancel_button, 0, 2, 1, 1);
  grid.attach(add_button, 1, 2, 1, 1);
  hbox.pack_start(grid, TRUE, TRUE, 10);
  vbox.pack_start(hbox, TRUE, TRUE, 10);
  add(vbox);

  //Connect signal handler:
  add_button.signal_clicked().connect( sigc::mem_fun(*this, &AddWindow::added) );
  cancel_button.signal_clicked().connect( sigc::mem_fun(*this, &AddWindow::hide) );

  show_all_children();
}

AddWindow::~AddWindow()
{
}

void AddWindow::on_cell_data_extra(const Gtk::TreeModel::const_iterator& iter)
{
  auto row = *iter;
  const Glib::ustring extra = row[m_Columns.m_col_extra];

  //Some arbitrary logic just to show that this is where you can do such things:

  //Transform the value, deciding how to represent it as text:
  if(extra.empty())
    m_cell.property_text() = "(none)";
  else
    m_cell.property_text() = "-" + extra + "-";

  //Change other cell renderer properties too:
  m_cell.property_foreground() = (extra == "yadda" ? "red" : "green");
}

void AddWindow::added(){
  Gtk::TreeModel::iterator iter = m_Combo.get_active();
  if(iter)
  {
    Gtk::TreeModel::Row row = *iter;
    if(row)
    {
      //Get the data for the selected row, using our knowledge of the tree
      //model:
      int id = row[m_Columns.m_col_id];
      Glib::ustring name = row[m_Columns.m_col_name];

      std::cout << name << command.get_text() << std::endl;
      l->add_gesture(g[id], command.get_text());
      hide();
    }
  }
  else
    std::cout << "invalid iter" << std::endl;
}