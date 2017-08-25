/*
 * ansvif
 * A Not So Very Intelligent Fuzzer
 * Marshall Whittaker / oxagast
 */

//    __ _  _  __   ___  __  ____ ____
//   /  ( \/ )/ _\ / __)/ _\/ ___(_  _)
//  (  O )  (/    ( (_ /    \___ \ )(
//   \__(_/\_\_/\_/\___\_/\_(____/(__)

/* this code does not work at all yet! */

#include <gtk/gtk.h>
#include <iostream>
#include <string>
#include <unistd.h>

std::string buffer_size = " -b 32 ";
std::string template_file = "";

int fuzz_call() {
  std::string ansvif_call = "./ansvif " + buffer_size + template_file;
  std::cout << ansvif_call << std::endl;
  return (0);
}

static void template_selected(GtkWidget *w, GtkFileSelection *fs) {
  template_file.assign(gtk_file_selection_get_filename(GTK_FILE_SELECTION(fs)));
  template_file = " -t " + template_file;
}

int select_template() {
  GtkWidget *templ_file;
  templ_file = gtk_file_selection_new("File selection");
  g_signal_connect(templ_file, "destroy", G_CALLBACK(gtk_main_quit), NULL);
  g_signal_connect(GTK_FILE_SELECTION(templ_file)->ok_button, "clicked",
                   G_CALLBACK(template_selected), (gpointer)templ_file);
  g_signal_connect_swapped(GTK_FILE_SELECTION(templ_file)->cancel_button,
                           "clicked", G_CALLBACK(gtk_widget_destroy),
                           templ_file);
  gtk_file_selection_set_filename(GTK_FILE_SELECTION(templ_file),
                                  "penguin.png");
  gtk_widget_show(templ_file);
  gtk_main();
  return 0;
}

int set_buffer_size(GtkWidget *buf_size_zero, gpointer data) {
  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(buf_size_zero))) {
    buffer_size = " -y ";
  } else {
    buffer_size = " -b 32 ";
  }
  return (0);
}

void destroy(GtkWidget *widget, gpointer data) { gtk_main_quit(); }

int main(int argc, char *argv[]) {
  GtkWidget *window;
  GtkWidget *opters;
  GtkWidget *fuzz_it, *template_sel;
  GtkWidget *buf_size_zero;
  gtk_init(&argc, &argv);
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "ansvif");
  g_signal_connect(window, "destroy", G_CALLBACK(destroy), NULL);
  gtk_container_set_border_width(GTK_CONTAINER(window), 15);
  opters = gtk_fixed_new();
  gtk_container_add(GTK_CONTAINER(window), opters);
  fuzz_it = gtk_button_new_with_label("Fuzz!");
  g_signal_connect(GTK_OBJECT(fuzz_it), "clicked", G_CALLBACK(fuzz_call),
                   "fuzz_it");
  gtk_fixed_put(GTK_FIXED(opters), fuzz_it, 400, 10);
  gtk_widget_show(fuzz_it);
  gtk_widget_show(opters);
  template_sel = gtk_button_new_with_label("Select Template");
  g_signal_connect(GTK_OBJECT(template_sel), "clicked",
                   G_CALLBACK(select_template), "template_sel");
  gtk_fixed_put(GTK_FIXED(opters), template_sel, 50, 50);
  gtk_widget_show(template_sel);
  buf_size_zero = gtk_toggle_button_new_with_label("Buffer Size 0");
  g_signal_connect(GTK_OBJECT(buf_size_zero), "clicked",
                   G_CALLBACK(set_buffer_size), "buf_size_zero");
  gtk_fixed_put(GTK_FIXED(opters), buf_size_zero, 50, 80);
  gtk_widget_show(buf_size_zero);
  gtk_widget_show(opters);
  gtk_widget_show_all(window);
  gtk_main();
  return 0;
}
