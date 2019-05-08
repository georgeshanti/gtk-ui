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
#include <gtkmm/comboboxtext.h>
#include <gtkmm/liststore.h>
#include <gtkmm/cssprovider.h>
#include <string>
#include <regex>
#include <iostream>
#include <fstream>

class CameraFeed : public Gtk::Box{
  public:
    Gtk::Image img;
    Gtk::Box hbox;

    CameraFeed():Gtk::Box(Gtk::ORIENTATION_VERTICAL), hbox(), img(){

      Gdk::RGBA orange;
      orange.set_rgba((double)241/255,(double)132/255,(double)88/255);
      override_background_color(orange);

      hbox.pack_start(img, TRUE, TRUE, 5);
      pack_start(hbox, TRUE, TRUE, 0);
      img.show();
      hbox.show();
    }
};

class Callibration : public Gtk::Box{
  public:
    Gtk::Grid callibration_grid;
    Gtk::HScale h_max_scale, h_min_scale, s_max_scale, s_min_scale, v_max_scale, v_min_scale;
    Gtk::Label h_label, s_label, v_label;
    Gtk::Box ch_box, cv_box, cbh_box;
    std::map<char,std::pair<int,int>> *hsv_values;

    Callibration(std::map<char,std::pair<int,int>>*);
    void draw();
};

class Gesture{
  public:
    int fingers, direction;
    Gesture():fingers(0), direction(0){}
    Gesture(int f, int d):fingers(f), direction(d){}
    
    std::string name() const;

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

class List;

class EditWindow : public Gtk::Window
{
public:
  EditWindow(Gesture, std::string, List*);
  virtual ~EditWindow();
  Gesture ges;
  Gtk::Label label;
  Gtk::Grid grid;
  Gtk::Entry command;
  Gtk::Button add_button, cancel_button;
  Gtk::Box vbox,hbox;
  List *l;
  void saved();
};

class AddWindow : public Gtk::Window
{
public:
  AddWindow(std::vector<Gesture>, List*);
  virtual ~AddWindow();
  Gtk::Grid grid;
  Gtk::Entry command;
  Gtk::Button add_button, cancel_button;
  Gtk::Box vbox,hbox;
  List *l;
  std::map<int, Gesture> g;

protected:
  void on_cell_data_extra(const Gtk::TreeModel::const_iterator& iter);

  //Signal handlers:
  void on_combo_changed();
  void added();


  //Tree model columns:
  class ModelColumns : public Gtk::TreeModel::ColumnRecord
  {
  public:

    ModelColumns()
    { add(m_col_id); add(m_col_name); add(m_col_extra);}

    Gtk::TreeModelColumn<int> m_col_id;
    Gtk::TreeModelColumn<Glib::ustring> m_col_name;
    Gtk::TreeModelColumn<Glib::ustring> m_col_extra;
  };

  ModelColumns m_Columns;

  //Child widgets:
  Gtk::ComboBox m_Combo;
  Gtk::CellRendererText m_cell;
  Glib::RefPtr<Gtk::ListStore> m_refTreeModel;
};

class List : public Gtk::Box{
  public:
    Gtk::Grid list_grid;
    Gtk::Button add_button, edit_button, remove_button;
    Gtk::ListBox list;
    Gtk::Box lbh_box, lbv_box;
    Gtk::ScrolledWindow list_window;
    std::map<int, Gtk::ListBoxRow> list_map;
    std::map<int, Gesture> gesture_map;
    std::map<Gesture, std::string> gestures;
    AddWindow *add_window;
    EditWindow *edit_window;
    Gtk::Window *parent_window;

    void draw();
    void delete_gesture();
    void add_gesture(Gesture g, std::string s){
      gestures[g]=s;
      std::cout<<"Added gesture: "<<gestures[g]<<std::endl;
      draw();
    }
    List();
    void parent(Gtk::Window* a){
      parent_window=a;
    }
    void aboutWinClose();
    void open_add_window();
    void open_edit_window();
};

class GuiSettings : public Gtk::Window
{

public:
  GuiSettings();
  virtual ~GuiSettings();

  //Member widgets:
  CameraFeed img;
  Callibration *callibration;
  List list_box;
  Gtk::Button call, set, cancel, save;
  bool set_frame = FALSE;
  bool get_color = FALSE;

  Gtk::Grid window_grid;
  std::map<char,std::pair<int,int>> hsv_values;
  void call_colors();
  void set_colors();
  void save_func();

};

#endif // GTKMM_EXAMPLE_HELLOWORLD_H