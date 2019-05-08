#include "GuiSettings.hpp"

GestureItem::GestureItem(Gesture g, std::string c):_gesture(g), command(c), grid(), vbox(Gtk::ORIENTATION_VERTICAL), hbox(){
  gesture = Gtk::Label();
  gesture.set_markup("<span>"+_gesture.name()+"</span>");
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