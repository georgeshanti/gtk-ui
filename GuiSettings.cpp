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

void GuiSettings::on_button_clicked()
{
  std::cout<<"Entered\n";
  if(add_window != 0)
    return;
  std::cout<<"Add window\n";
  add_window = new AddWindow;
  add_window->signal_hide().connect(sigc::mem_fun(*this, &GuiSettings::aboutWinClose));
  add_window->show();
}

void GuiSettings::aboutWinClose()
{
    add_window = 0;
}

GuiSettings::GuiSettings(): img(), window_grid(), callibration(),
hsv_values()
{
  add_window = 0;

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

  // h_min_scale.set_range(0,179);
  // h_min_scale.set_value(hsv_values['h'].first);
  // h_max_scale.set_range(0,179);
  // h_max_scale.set_value(hsv_values['h'].second);

  // s_min_scale.set_range(0,255);
  // s_min_scale.set_value(hsv_values['s'].first);
  // s_max_scale.set_range(0,255);
  // s_max_scale.set_value(hsv_values['s'].second);

  // v_min_scale.set_range(0,255);
  // v_min_scale.set_value(hsv_values['v'].first);
  // v_max_scale.set_range(0,255);
  // v_max_scale.set_value(hsv_values['v'].second);

  // list.spa

  // list.set_border_width(3);

  window_grid.attach(img,0,0,1,1);

  Gdk::RGBA w;
  w.set_rgba(1,1,1);

  Gdk::RGBA orange;
  orange.set_rgba((double)241/255,(double)132/255,(double)88/255);

  // add_button.signal_clicked().connect( sigc::mem_fun(*this, &GuiSettings::on_button_clicked) );


  window_grid.attach(callibration, 0, 1, 1,1);
  callibration.show();

  window_grid.attach(list_box, 1,0,1,2);
  list_box.show();

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

#endif