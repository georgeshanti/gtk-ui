#include "GuiSettings.hpp"

Callibration::Callibration():Gtk::Box(Gtk::ORIENTATION_VERTICAL), h_label(), s_label(), v_label(),
h_min_scale(), s_min_scale(), v_min_scale(),
h_max_scale(), s_max_scale(), v_max_scale(),
ch_box(), cv_box(Gtk::ORIENTATION_VERTICAL), cbh_box(){

  Gdk::RGBA orange;
  orange.set_rgba((double)241/255,(double)132/255,(double)88/255);
  
  Gdk::RGBA w;
  w.set_rgba(1,1,1);

  h_label.set_markup("<span foreground='#f18458' size='large'>Hue</span>");
  callibration_grid.attach(h_label, 0, 0, 1, 1);
  h_min_scale.set_range(0,179);
  callibration_grid.attach(h_min_scale, 0, 1, 1, 1);
  h_max_scale.set_range(0,179);
  callibration_grid.attach(h_max_scale, 0, 2, 1, 1);

  s_label.set_markup("<span foreground='#f18458' size='large'>Saturation</span>");
  callibration_grid.attach(s_label, 1, 0, 1, 1);
  s_min_scale.set_range(0,255);
  callibration_grid.attach(s_min_scale, 1, 1, 1, 1);
  s_max_scale.set_range(0,255);
  callibration_grid.attach(s_max_scale, 1, 2, 1, 1);
  
  v_label.set_markup("<span foreground='#f18458' size='large'>Value</span>");
  callibration_grid.attach(v_label, 2, 0, 1, 1);
  v_min_scale.set_range(0,255);
  callibration_grid.attach(v_min_scale, 2, 1, 1, 1);
  v_max_scale.set_range(0,255);
  callibration_grid.attach(v_max_scale, 2, 2, 1, 1);

  callibration_grid.set_column_spacing(10);
  callibration_grid.set_row_spacing(10);
  callibration_grid.set_column_homogeneous(TRUE);

  ch_box.pack_start(callibration_grid, TRUE, TRUE, 10);

  cv_box.override_background_color(w);
  cv_box.pack_start(ch_box, TRUE, TRUE, 10);

  cbh_box.pack_start(cv_box, TRUE, TRUE, 2);

  override_background_color(orange);
  pack_start(cbh_box, TRUE, TRUE, 2);

  h_label.show();
  s_label.show();
  v_label.show();

  h_min_scale.show();
  s_min_scale.show();
  v_min_scale.show();

  h_max_scale.show();
  s_max_scale.show();
  v_max_scale.show();

  callibration_grid.show();
  ch_box.show();
  cv_box.show();
  cbh_box.show();
}