#include "GuiSettings.hpp"
#include<iostream>
std::regex num("[0-9]+");
std::regex alpha("[alpha]+");

void List::draw(){
  list_window.remove();
  list = Gtk::ListBox();

  list_map.clear();
  gesture_map.clear();
  int i=0;
  for(auto s=gestures.begin();s!=gestures.end();s++){
    if(s->second=="")
      continue;
    auto row = new GestureItem(s->first, s->second);
    
    gesture_map[i] = s->first;
    list.append(*row);
    i++;
  }
  list.show_all();

  list_window.add(list);

}

void List::delete_gesture(){
  auto row = list.get_selected_row();
  int i = row->get_index();
  gestures.erase(gesture_map[i]);
  
  draw();
}

void List::open_add_window()
{
  std::cout<<"Entered\n";
  if(add_window != 0)
    return;
  std::cout<<"Add window\n";

  std::vector<Gesture> ges;
  for(auto s=gestures.begin();s!=gestures.end();s++){
    if(s->second!="")
      continue;
    ges.push_back(s->first);
  }
  
  add_window = new AddWindow(ges, this);
  add_window->signal_hide().connect(sigc::mem_fun(*this, &List::aboutWinClose));
  add_window->show();
}

void List::open_edit_window()
{
  auto row = list.get_selected_row();
  int i = row->get_index();
  Gesture g = gesture_map[i];
  edit_window = new EditWindow(g, gestures[g], this);
  edit_window->signal_hide().connect(sigc::mem_fun(*this, &List::aboutWinClose));
  edit_window->show();
}

void List::aboutWinClose()
{
  delete add_window;
  add_window = 0;
  delete edit_window;
  edit_window = 0;
}

List::List(): Gtk::Box(Gtk::ORIENTATION_VERTICAL), lbh_box(), gestures(), list_window(),
add_button("ADD"), edit_button("EDIT"), remove_button("REMOVE"), list_grid(), list(), list_map(), gesture_map(){

  add_button.signal_clicked().connect( sigc::mem_fun(*this, &List::open_add_window) );
  edit_button.signal_clicked().connect( sigc::mem_fun(*this, &List::open_edit_window) );
  remove_button.signal_clicked().connect( sigc::mem_fun(*this, &List::delete_gesture) );

  add_window = 0;
  edit_window = 0;

  Gdk::RGBA orange;
  orange.set_rgba((double)241/255,(double)132/255,(double)88/255);

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

  draw();

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
  override_background_color(orange);
  pack_start(lbh_box, TRUE, TRUE, 2);

  lbv_box.show();
  lbh_box.show();

  // list.show_all();
}