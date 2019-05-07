#ifndef GTKMM_EXAMPLE_HELLOWORLD_H
#define GTKMM_EXAMPLE_HELLOWORLD_H

#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm/image.h>
#include <gtkmm/grid.h>
#include <gtkmm/scale.h>
#include <gtkmm/label.h>
#include <gtkmm/listbox.h>
#include <gtkmm/box.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/eventbox.h>
#include <gtkmm/combobox.h>
#include <gtkmm/textview.h>
#include <string>

class Gesture{
  public:
    int fingers, direction;
    Gesture():fingers(0), direction(0){}
    Gesture(int f, int d):fingers(f), direction(d){}

    bool operator <(const Gesture& d) const{
      if(fingers!=d.fingers)
        return fingers<d.fingers;
      else
        return direction<d.direction;
    }
};

class GestureItem : public Gtk::ListBoxRow{
  public:
    Gesture _gesture;
    std::string command;
    GestureItem(Gesture, std::string);
    virtual ~GestureItem();
    void show();
    Gtk::Box vbox, hbox;
    Gtk::Grid grid;
    Gtk::Label gesture, function;
};

class AddWindow : public Gtk::Window{
  public:
    std::vector<Gesture> gestures;
    Gtk::ComboBox dropdown;
    Gtk::TextView field;
    Gtk::Button add, cancel;

  //Tree model columns:
  // class ModelColumns : public Gtk::TreeModel::ColumnRecord
  // {
  // public:

  //   ModelColumns()
  //   { add(m_col_id); add(m_col_name); }

  //   Gtk::TreeModelColumn<int> m_col_id;
  //   Gtk::TreeModelColumn<Glib::ustring> m_col_name;
  // };

    AddWindow(){

    };
    
};

class GuiSettings : public Gtk::Window
{

public:
  GuiSettings();
  virtual ~GuiSettings();

  //Member widgets:
  Gtk::Image img;
  Gtk::Grid window_grid, callibration_grid, list_grid;
  Gtk::Button add_button, edit_button, remove_button;
  Gtk::HScale h_max_scale, h_min_scale, s_max_scale, s_min_scale, v_max_scale, v_min_scale;
  Gtk::Label h_label, s_label, v_label;
  Gtk::ListBox list;
  Gtk::Box ch_box, cv_box, cbh_box, cbv_box,
    lbh_box, lbv_box;
  Gtk::ScrolledWindow list_window;
  std::map<char,std::pair<int,int>> hsv_values;
  std::map<Gesture, std::string> gestures;

protected:
  //Signal handlers:
  void on_button_clicked();

};

#endif // GTKMM_EXAMPLE_HELLOWORLD_H