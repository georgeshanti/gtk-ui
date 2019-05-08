#include "GuiSettings.hpp"
#include <iostream>

EditWindow::EditWindow(Gesture g, std::string s, List *li):ges(g), add_button("SAVE"), cancel_button("CANCEL"), grid(), command(),
hbox(), vbox(Gtk::ORIENTATION_VERTICAL)
{
  l=li;
  label.set_text(ges.name());
  command.set_text(s);
  set_title("Edit Gesture");
  //Add the ComboBox to the window.
  grid.set_column_homogeneous(TRUE);
  grid.set_row_spacing(10);
  grid.set_column_spacing(10);

  grid.attach(label, 0, 0, 4, 1);
  grid.attach(command, 0, 1, 4, 1);
  grid.attach(cancel_button, 2, 2, 1, 1);
  grid.attach(add_button, 3, 2, 1, 1);
  hbox.pack_start(grid, TRUE, TRUE, 10);
  vbox.pack_start(hbox, TRUE, TRUE, 10);
  add(vbox);

  //Connect signal handler:
  add_button.signal_clicked().connect( sigc::mem_fun(*this, &EditWindow::saved) );
  cancel_button.signal_clicked().connect( sigc::mem_fun(*this, &EditWindow::hide) );

  show_all_children();
}

EditWindow::~EditWindow()
{
}

void EditWindow::saved(){
    l->add_gesture(ges, command.get_text());
    hide();
}