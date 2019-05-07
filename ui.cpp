#include <gtk/gtk.h>
#include <opencv2/opencv.hpp>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <glib.h>
#include <iostream>

/* This is the callback function. 
 * It is a handler function which reacts to the signal. 
 * In this case, it will notify the user the value of their scale as a label.
 */


  /* Declare variables */
  GtkWidget *window;
  GtkWidget *h_min_scale;
  GtkWidget *h_max_scale;
  GtkWidget *s_min_scale;
  GtkWidget *s_max_scale;
  GtkWidget *v_min_scale;
  GtkWidget *v_max_scale;
  GtkWidget *hlabel;
  GtkWidget *slabel;
  GtkWidget *vlabel;
  GtkWidget *hsv_grid, *window_grid;
  GtkWidget *image;

  GdkPixbuf *buf;

  /* The Adjustment object represents a value 
   * which has an associated lower and upper bound.
   */
  GtkAdjustment *hadjustment_min, *hadjustment_max, *sadjustment_min, *sadjustment_max, *vadjustment_min, *vadjustment_max;

  static cairo_surface_t *surface = NULL;

void MatToCairo(cv::Mat &MC3,cairo_surface_t *surface)
{
  cv::Mat MC4 = cv::Mat(cairo_image_surface_get_width(surface),cairo_image_surface_get_height(surface),CV_8UC4,cairo_image_surface_get_data(surface),cairo_image_surface_get_stride(surface));
  std::vector<cv::Mat> Imgs1;
  std::vector<cv::Mat> Imgs2;
  cv::split(MC4,Imgs1);
  cv::split(MC3,Imgs2);
  for(int i=0;i<3;i++)
  {
  Imgs1[i]=Imgs2[i];
  }
  // Alpha - прозрачность
  Imgs1[3]=255; 
  cv::merge(Imgs1,MC4);
}

static void do_drawing(cairo_t *cr)
{
  // cairo_set_source_surface(cr, buf, 10, 10);
  std::cout<<"Fuuuuck\n";
  cairo_set_source_surface(cr, surface, 0,0);
  // gdk_cairo_set_source_pixbuf(cr, buf, 0, 0);
  cairo_paint(cr);    
}

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, 
    gpointer user_data)
{      
  do_drawing(cr);

  return FALSE;
}

static void
hscale_moved (GtkRange *range,
              gpointer  user_data)
{
   GtkWidget *label = (GtkWidget*)user_data;

   /* Get the value of the range, and convert it into a string which will be
    * used as a new label for the horizontal scale.
    * %.0f - stands for a double that will have 0 decimal places.
    */
   gdouble pos = gtk_range_get_value (range);
   /* Note: Using g_strdup_printf returns a string that must be freed. 
    * (In which is done below)
    */
   gchar *str = g_strdup_printf ("Horizontal scale is %.0f", pos);
   gtk_label_set_text (GTK_LABEL (label), str);

   g_free(str);
}

static void
activate (GtkApplication *app,
          gpointer        user_data)
{

  /* Create a window with a title and a default size */
  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Scale Example");
  gtk_window_set_default_size (GTK_WINDOW (window), 400, 300);
  gtk_container_set_border_width (GTK_CONTAINER (window), 5);

  /* Two labels to be shown in the window */
  hlabel = gtk_label_new ("Hue");
  slabel = gtk_label_new ("Saturation");
  vlabel = gtk_label_new ("Value");

   
  /* gtk_adjustment_new takes six parameters, three of which 
   * may be difficult to understand:
   * step increment- move the handle with the arrow keys on your keyboard to see.
   * page increment - move the handle by clicking away from it 
   * on the scale to see.
   * page size - not used here.
   */
  hadjustment_min = gtk_adjustment_new (0, 0, 179, 5, 10, 0);
  hadjustment_max = gtk_adjustment_new (179, 0, 179, 5, 10, 0);

  sadjustment_min = gtk_adjustment_new (0, 0, 255, 5, 10, 0);
  sadjustment_max = gtk_adjustment_new (255, 0, 255, 5, 10, 0);

  vadjustment_min = gtk_adjustment_new (0, 0, 255, 5, 10, 0);
  vadjustment_max = gtk_adjustment_new (255, 0, 255, 5, 10, 0);

  /* Create the Horizontal scale, making sure the 
   * digits used have no decimals.
   */
  h_min_scale = gtk_scale_new (GTK_ORIENTATION_HORIZONTAL, hadjustment_min);
  gtk_scale_set_digits (GTK_SCALE (h_min_scale), 0);
  h_max_scale = gtk_scale_new (GTK_ORIENTATION_HORIZONTAL, hadjustment_max);
  gtk_scale_set_digits (GTK_SCALE (h_max_scale), 0); 

  s_min_scale = gtk_scale_new (GTK_ORIENTATION_HORIZONTAL, sadjustment_min);
  gtk_scale_set_digits (GTK_SCALE (s_min_scale), 0);
  s_max_scale = gtk_scale_new (GTK_ORIENTATION_HORIZONTAL, sadjustment_max);
  gtk_scale_set_digits (GTK_SCALE (s_max_scale), 0); 
  
  v_min_scale = gtk_scale_new (GTK_ORIENTATION_HORIZONTAL, vadjustment_min);
  gtk_scale_set_digits (GTK_SCALE (v_min_scale), 0);
  v_max_scale = gtk_scale_new (GTK_ORIENTATION_HORIZONTAL, vadjustment_max);
  gtk_scale_set_digits (GTK_SCALE (v_max_scale), 0); 

  /* Allow it to expand horizontally (if there's space), and 
   * set the vertical alignment
   */
  gtk_widget_set_hexpand (h_min_scale, TRUE);
  gtk_widget_set_valign (h_min_scale, GTK_ALIGN_START);
  gtk_widget_set_hexpand (h_max_scale, TRUE);
  gtk_widget_set_valign (h_max_scale, GTK_ALIGN_START);

  gtk_widget_set_hexpand (s_min_scale, TRUE);
  gtk_widget_set_valign (s_min_scale, GTK_ALIGN_START);
  gtk_widget_set_hexpand (s_max_scale, TRUE);
  gtk_widget_set_valign (s_max_scale, GTK_ALIGN_START);

  gtk_widget_set_hexpand (v_min_scale, TRUE);
  gtk_widget_set_valign (v_min_scale, GTK_ALIGN_START);
  gtk_widget_set_hexpand (v_max_scale, TRUE);
  gtk_widget_set_valign (v_max_scale, GTK_ALIGN_START);
  
  /* Connecting the "value-changed" signal for the horizontal scale 
   * to the appropriate callback function. 
   * take note that GtkRange is part of GtkScale's Object Hierarchy.
   */
  g_signal_connect (h_min_scale, 
                    "value-changed", 
                    G_CALLBACK (hscale_moved), 
                    hlabel);
  g_signal_connect (h_max_scale, 
                    "value-changed", 
                    G_CALLBACK (hscale_moved), 
                    hlabel);
  g_signal_connect (s_min_scale, 
                    "value-changed", 
                    G_CALLBACK (hscale_moved), 
                    slabel);
  g_signal_connect (s_max_scale, 
                    "value-changed", 
                    G_CALLBACK (hscale_moved), 
                    slabel);
  g_signal_connect (v_min_scale, 
                    "value-changed", 
                    G_CALLBACK (hscale_moved), 
                    vlabel);
  g_signal_connect (v_max_scale, 
                    "value-changed", 
                    G_CALLBACK (hscale_moved), 
                    vlabel);



  /* Create the Vertical scale. This time, we will see what happens 
   * when the digits aren't initially set.
   */

  /* Connecting the "value-changed" signal for the vertical scale to 
   * the appropriate callback function.
   */

  /* Create a grid and arrange everything accordingly */

  window_grid = gtk_grid_new ();
  gtk_grid_set_column_spacing (GTK_GRID (window_grid), 10);
  gtk_grid_set_column_homogeneous (GTK_GRID (window_grid), TRUE);

  hsv_grid = gtk_grid_new ();
  gtk_grid_set_column_spacing (GTK_GRID (hsv_grid), 10);
  gtk_grid_set_column_homogeneous (GTK_GRID (hsv_grid), TRUE);

  gtk_grid_attach (GTK_GRID (hsv_grid), hlabel, 0, 0, 1, 1);
  gtk_grid_attach (GTK_GRID (hsv_grid), h_min_scale, 0, 1, 1, 1);
  gtk_grid_attach (GTK_GRID (hsv_grid), h_max_scale, 0, 2, 1, 1);

  gtk_grid_attach (GTK_GRID (hsv_grid), slabel, 1, 0, 1, 1);
  gtk_grid_attach (GTK_GRID (hsv_grid), s_min_scale, 1, 1, 1, 1);
  gtk_grid_attach (GTK_GRID (hsv_grid), s_max_scale, 1, 2, 1, 1);

  gtk_grid_attach (GTK_GRID (hsv_grid), vlabel, 2, 0, 1, 1);
  gtk_grid_attach (GTK_GRID (hsv_grid), v_min_scale, 2, 1, 1, 1);
  gtk_grid_attach (GTK_GRID (hsv_grid), v_max_scale, 2, 2, 1, 1);


  image = gtk_image_new();
  gtk_grid_attach (GTK_GRID (window_grid), hsv_grid, 0, 1, 1, 1);
  


  cv::VideoCapture cap(0); // open the default camera
  if(!cap.isOpened())  // check if we succeeded
    return;
  cv::Mat frame;
  cap>>frame;
  // cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
  GtkWidget *darea = gtk_drawing_area_new();

  g_signal_connect(G_OBJECT(darea), "draw", 
      G_CALLBACK(on_draw_event), NULL);

  buf = gdk_pixbuf_new_from_data(frame.data, GDK_COLORSPACE_RGB, false, 8, frame.cols, frame.rows, frame.step, NULL, NULL);
  cairo_format_t format;
  int width = gdk_pixbuf_get_width(buf);
  int height = gdk_pixbuf_get_height(buf);
  format = (gdk_pixbuf_get_has_alpha(buf))? CAIRO_FORMAT_ARGB32 : CAIRO_FORMAT_RGB24;
  surface = cairo_image_surface_create(format, width, height);
  MatToCairo(frame, surface);
  // cairo_t *cr = cairo_create(surface);
  // gdk_cairo_set_source_pixbuf(cr, buf, 0, 0);
  // cairo_paint(cr);
  // GtkImage *img = (GtkImage*)gtk_image_new();
  // gtk_image_set_from_pixbuf(img,buf);
  // if(image==NULL) std::cout<<"null image";
  // GError *error=NULL;
  // gdk_pixbuf_save (buf, "file", "jpeg", &error, "quality", "100", NULL);
  // GtkWidget *img = gtk_image_new_from_file ("file");
  // cv::imshow("frame",frame);
  gtk_grid_attach (GTK_GRID (window_grid), darea, 0, 0, 1, 1);
  // gtk_container_add (GTK_CONTAINER (window), img);

  gtk_container_add (GTK_CONTAINER (window), window_grid);

  gtk_widget_show_all (window);
}



int
main (int argc, char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}