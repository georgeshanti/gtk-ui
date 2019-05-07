#include "GuiSettings.hpp"
#include <gtkmm-3.0/gtkmm/object.h>
#include <iostream>
#include <fstream>
#include <regex>
#include <gtkmm/alignment.h>
#include <pangomm-1.4/pangomm.h>
#include <glib.h>
#include <gtkmm/cssprovider.h>
// #include <gtkmm/color.h>
// #inclu

std::regex num("[0-9]+");
std::regex alpha("[alpha]+");

std::string fin[4] = {"One Finger", "Two Finger", "Three Finger", "Four Finger"};
std::string dir[4] = {"Left Swipe", "Right Swipe", "Up Swipe", "Down Swipe"};

void GuiSettings::on_button_clicked()
{
    std::cout << "Hello World" << std::endl;
}

GestureItem::GestureItem(Gesture g, std::string c):_gesture(g), command(c), grid(), vbox(Gtk::ORIENTATION_VERTICAL), hbox(){
  gesture = Gtk::Label();
  gesture.set_markup("<span>"+fin[_gesture.fingers-1]+std::string(" ")+dir[_gesture.direction-1]+"</span>");
  function = Gtk::Label();

  int a,pos=0;
  a=c.find("&",pos);
  while(a!=std::string::npos){
    c.insert(a+1, "amp;");
    pos=a+1;
    a=c.find("&",pos);
  }

  function.set_markup("<span size='x-large'>"+c+"</span>");
  function.set_max_width_chars(100);
  function.set_ellipsize(Pango::ELLIPSIZE_MIDDLE);

  gesture.set_halign(Gtk::ALIGN_START);
  function.set_halign(Gtk::ALIGN_START);


  // Pango::FontDescription fd1;
  // fd1.set_size(30);
  // Pango::Attribute attr1 = Pango::Attribute::create_attr_font_desc(fd1);
  // Pango::AttrList labelAttributes1;
  // labelAttributes1.insert( attr1 );
  // gesture.set_attributes( labelAttributes1 );
  // gesture.set_text(fin[_gesture.fingers-1]+std::string(" ")+dir[_gesture.direction-1]);

  // grid.set_row_spacing(5);
  grid.set_halign(Gtk::ALIGN_START);

  grid.attach(gesture,0,0,1,1);
  grid.attach(function,0,1,1,1);
  
  vbox.pack_start(grid, TRUE, TRUE, 10);
  hbox.pack_start(vbox, TRUE, TRUE, 20);
  add(hbox);

  gesture.show();
  function.show();
  grid.show();
}

GestureItem::~GestureItem(){

}

GuiSettings::GuiSettings()
: img(), window_grid(), h_max_scale(), h_min_scale(),
h_label("Hue"), s_label("Saturation"), v_label("Value"),
list(), callibration_grid(), ch_box(), cv_box(Gtk::ORIENTATION_VERTICAL), cbh_box(), cbv_box(Gtk::ORIENTATION_VERTICAL),
lbh_box(), lbv_box(Gtk::ORIENTATION_VERTICAL), hsv_values(), gestures(), list_window(), add_button("ADD"), edit_button("EDIT"),
remove_button("REMOVE"), list_grid()
{

  std::ifstream config;
  config.open("./config.txt");
  std::string setting;
  std::smatch m;
  while(getline(config, setting)){

    char f = setting[0];

    setting=setting.substr(2);
    int low;
    std::stringstream(setting)>>low;

    int i=1,tmp=low;
    while(tmp>9){
      tmp=tmp/10;
      i++;
    }

    setting = setting.substr(i+1);
    int high;
    std::stringstream(setting)>>high;
    hsv_values[f] = std::pair<int,int>(low,high);
    std::cout<<f<<":"<<low<<":"<<high<<std::endl;
  }

  // Sets the border width of the window.
  set_border_width(10);
  window_grid.set_row_spacing(10);
  window_grid.set_column_spacing(10);

  h_min_scale.set_range(0,179);
  h_min_scale.set_value(hsv_values['h'].first);
  h_max_scale.set_range(0,179);
  h_max_scale.set_value(hsv_values['h'].second);

  s_min_scale.set_range(0,255);
  s_min_scale.set_value(hsv_values['s'].first);
  s_max_scale.set_range(0,255);
  s_max_scale.set_value(hsv_values['s'].second);

  v_min_scale.set_range(0,255);
  v_min_scale.set_value(hsv_values['v'].first);
  v_max_scale.set_range(0,255);
  v_max_scale.set_value(hsv_values['v'].second);

  // list.spa

  // list.set_border_width(3);

  std::ifstream saved;
  saved.open("./saved_gestures.txt");
  std::string line;

  for(int i=1;i<=4;i++){
    for(int j=1;j<=4;j++){
      Gesture g(i,j);
      gestures[g]="";
    }
  }

  while(getline(saved, line)){
    std::smatch m;
    std::regex_search(line,m,num);
    int fingers;
    std::stringstream(m.str())>>fingers;
    line = m.suffix();
    std::regex_search(line,m,num);
    int direction;
    std::stringstream(m.str())>>direction;
    line = m.suffix();
    line = line.substr(2,line.length()-3);

    Gesture g(fingers, direction);
    gestures[g]=line;
  }

  for(auto s=gestures.begin();s!=gestures.end();s++){
    if(s->second=="")
      continue;
    auto row = new GestureItem(s->first, s->second);
    list.append(*row);
  }
  
  h_label.set_markup("<span foreground='#f18458' size='large'>Hue</span>");
  callibration_grid.attach(h_label, 0, 0, 1, 1);
  callibration_grid.attach(h_min_scale, 0, 1, 1, 1);
  callibration_grid.attach(h_max_scale, 0, 2, 1, 1);

  s_label.set_markup("<span foreground='#f18458' size='large'>Saturation</span>");
  callibration_grid.attach(s_label, 1, 0, 1, 1);
  callibration_grid.attach(s_min_scale, 1, 1, 1, 1);
  callibration_grid.attach(s_max_scale, 1, 2, 1, 1);
  
  v_label.set_markup("<span foreground='#f18458' size='large'>Value</span>");
  callibration_grid.attach(v_label, 2, 0, 1, 1);
  callibration_grid.attach(v_min_scale, 2, 1, 1, 1);
  callibration_grid.attach(v_max_scale, 2, 2, 1, 1);

  callibration_grid.set_column_spacing(10);
  callibration_grid.set_row_spacing(10);
  callibration_grid.set_column_homogeneous(TRUE);

  window_grid.attach(img,0,0,1,1);

  Gdk::RGBA w;
  w.set_rgba(1,1,1);

  Gdk::RGBA orange;
  orange.set_rgba((double)241/255,(double)132/255,(double)88/255);

  ch_box.pack_start(callibration_grid, TRUE, TRUE, 10);

  cv_box.override_background_color(w);
  cv_box.pack_start(ch_box, TRUE, TRUE, 10);

  cbh_box.pack_start(cv_box, TRUE, TRUE, 2);
  cbv_box.override_background_color(orange);
  cbv_box.pack_start(cbh_box, TRUE, TRUE, 2);

  add_button.signal_clicked().connect( sigc::mem_fun(*this, &GuiSettings::on_button_clicked) );

  cbv_box.show();
  cbh_box.show();

  window_grid.attach(cbv_box, 0, 1, 1,1);

  list_window.add(list);
  list_window.set_min_content_width(400);
  list_window.set_min_content_height(600);
  // list_window.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_ALWAYS);
  list_window.show();

  Glib::RefPtr<Gtk::CssProvider> css_provider = Gtk::CssProvider::create();
  css_provider->load_from_data(
    "button {outline: 0; color: white; background-image: image(#f18458);}\
     button:hover {background-image: image(#dd7044);}\
     button:active {background-image: image(#dd7044);}");

  // add_button.override_background_color(orange);
  add_button.get_style_context()->add_provider(
    css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);
  // add_button.set_relief(Gtk::RELIEF_NONE);
  add_button.show();

  edit_button.get_style_context()->add_provider(
    css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);
  // edit_button.set_relief(Gtk::RELIEF_NONE);
  edit_button.show();

  remove_button.get_style_context()->add_provider(
    css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);
  // edit_button.set_relief(Gtk::RELIEF_NONE);
  remove_button.show();

  list_grid.attach(list_window, 0,0,3,1);
  list_grid.attach(add_button, 0,1,1,1);
  list_grid.attach(edit_button, 1,1,1,1);
  list_grid.attach(remove_button, 2,1,1,1);
  list_grid.set_column_homogeneous(TRUE);
  // list_grid.set_hexpand(TRUE);
  // list_grid.set_vexpand(TRUE);
  list_grid.show();

  lbh_box.pack_start(list_grid, TRUE, TRUE, 2);
  lbv_box.override_background_color(orange);
  lbv_box.pack_start(lbh_box, TRUE, TRUE, 2);

  lbv_box.show();
  lbh_box.show();

  window_grid.attach(lbv_box, 1,0,1,2);


  Gdk::RGBA c;
  c.set_rgba((double)249/255,(double)110/255,(double)5/255);
  c.set_rgba((double)64/255,(double)64/255,(double)64/255);
  override_background_color(c);
  add(window_grid);
  set_default_size(1000,0);
  // set_resizable(FALSE);

  // The final step is to display this newly created widget...
  window_grid.show();

  img.show();
  callibration_grid.show();
  cv_box.show();
  ch_box.show();

  h_label.show();
  h_min_scale.show();
  h_max_scale.show();

  s_label.show();
  s_min_scale.show();
  s_max_scale.show();

  v_label.show();
  v_min_scale.show();
  v_max_scale.show();

  list.show_all();
}

GuiSettings::~GuiSettings()
{
}