#ifndef _GUI_SETTINGS_
#define _GUI_SETTINGS_

#include "GuiSettings.hpp"
#include <gtkmm-3.0/gtkmm/object.h>
#include <iostream>
#include <regex>
#include <gtkmm/alignment.h>
#include <pangomm-1.4/pangomm.h>
#include <glib.h>
#include <gtkmm/cssprovider.h>
// #include <gtkmm/color.h>

GuiSettings::GuiSettings(): img(), window_grid(),
hsv_values(), list_box(), call("CALLIBRATE"), set("SET COLORS"), save("SAVE")
{
  list_box.parent(this);
  callibration = new Callibration(&hsv_values);
  // add_window = 0;
  call.signal_clicked().connect( sigc::mem_fun(*this, &GuiSettings::call_colors) );
  set.signal_clicked().connect( sigc::mem_fun(*this, &GuiSettings::set_colors) );
  save.signal_clicked().connect( sigc::mem_fun(*this, &GuiSettings::save_func) );

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
    // std::cout<<f<<":"<<low<<":"<<high<<std::endl;
  }
  config.close();

  callibration->draw();

  // Sets the border width of the window.
  set_border_width(10);
  window_grid.set_row_spacing(10);
  window_grid.set_column_spacing(10);

  // list.spa

  // list.set_border_width(3);

  window_grid.attach(img,0,0,1,1);

  Gdk::RGBA w;
  w.set_rgba(1,1,1);

  Gdk::RGBA orange;
  orange.set_rgba((double)241/255,(double)132/255,(double)88/255);

  window_grid.attach(*callibration, 0, 1, 1,1);
  callibration->show();

  window_grid.attach(list_box, 1,0,4,2);
  list_box.show();

  window_grid.attach(set, 2,2,1,1);
  // set.show();
  window_grid.attach(call, 3,2,1,1);
  call.show();
  window_grid.attach(save, 4,2,1,1);
  save.show();


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

}

GuiSettings::~GuiSettings()
{
}

void GuiSettings::call_colors(){
  if(set_frame){
    set_frame = FALSE;
    set.hide();
    call.set_label("CALLIBRATE");
  }
  else{
    set_frame = TRUE;
    set.show();
    call.set_label("CANCEL");
  }
}

void GuiSettings::set_colors(){
  get_color = TRUE;
  set.show();
}

void GuiSettings::save_func(){
  std::ofstream config;
  config.open("./config.txt");
  for(auto s=hsv_values.begin() ; s!=hsv_values.end() ; s++){
    std::string setting = s->first + std::string(" ") + std::to_string(s->second.first) + std::string(" ") + std::to_string(s->second.second) + std::string("\n");
    config << setting;
  }
  config.close();


  std::ofstream saved_gestures;
  saved_gestures.open("./saved_gestures.txt");
  for(auto s=list_box.gestures.begin() ; s!=list_box.gestures.end() ; s++){
    if(s->second=="")
      continue;
    std::string setting = std::to_string(s->first.fingers) + std::string(" ") + std::to_string(s->first.direction) + std::string(" \"") + s->second + std::string("\"\n");
    saved_gestures << setting;
  }
  saved_gestures.close();
}

#endif