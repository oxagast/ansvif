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
std::string binary_file = "";

int fuzz_call() {
  /* put together the call to ansvif */
  std::string ansvif_call = "./ansvif " + buffer_size + binary_file + template_file + " & ";
  /* exec ansvif */
  execl("/bin/sh", "/bin/sh", "-c", ansvif_call.c_str(), NULL);  
  return (0);
}

static void template_selected(GtkWidget *w, GtkFileSelection *fs) {
  template_file.assign(gtk_file_selection_get_filename(GTK_FILE_SELECTION(fs)));
  template_file = " -t " + template_file;
}

static void binary_selected(GtkWidget *w, GtkFileSelection *fs) {
  binary_file.assign(gtk_file_selection_get_filename(GTK_FILE_SELECTION(fs)));
  binary_file = " -c " + binary_file;
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
                                  "examples/all_flags.cpp");
  gtk_widget_show(templ_file);
  gtk_main();
  return 0;
}

int select_binary() {
  GtkWidget *bin_file;
  bin_file = gtk_file_selection_new("File selection");
  g_signal_connect(bin_file, "destroy", G_CALLBACK(gtk_main_quit), NULL);
  g_signal_connect(GTK_FILE_SELECTION(bin_file)->ok_button, "clicked",
  G_CALLBACK(binary_selected), (gpointer)bin_file);
  g_signal_connect_swapped(GTK_FILE_SELECTION(bin_file)->cancel_button,
                          "clicked", G_CALLBACK(gtk_widget_destroy), bin_file);
  gtk_widget_show(bin_file);
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
  /* Declare some vars */
  std::string command;
  GtkWidget *window;
  GtkWidget *opters;
  GtkWidget *fuzz_it;
  GtkWidget *command_sel;
  GtkWidget *template_sel;
  GtkWidget *buf_size_zero;
  /* Pull in the args for gtk */
  gtk_init(&argc, &argv);
  /* Create gtk window */
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  /* Set the title to ansvif */
  gtk_window_set_title(GTK_WINDOW(window), "ansvif");
  g_signal_connect(window, "destroy", G_CALLBACK(destroy), NULL);
  gtk_container_set_border_width(GTK_CONTAINER(window), 15);
  /* Make a container for our buttons */
  opters = gtk_fixed_new();
  gtk_container_add(GTK_CONTAINER(window), opters);
  /* Make the allmighty fuzz button */
  fuzz_it = gtk_button_new_with_label("Fuzz!");
  g_signal_connect(GTK_OBJECT(fuzz_it), "clicked", G_CALLBACK(fuzz_call),
                   "fuzz_it");
  gtk_fixed_put(GTK_FIXED(opters), fuzz_it, 400, 10);
  gtk_widget_show(fuzz_it);
  /*  Make our template file selection */
  template_sel = gtk_button_new_with_label("Select Template");
  g_signal_connect(GTK_OBJECT(template_sel), "clicked",
                   G_CALLBACK(select_template), "template_sel");
  gtk_fixed_put(GTK_FIXED(opters), template_sel, 50, 80);
  gtk_widget_show(template_sel);
  /* Make our binary to be fuzzed file selection */
  command_sel = gtk_button_new_with_label("Select binary to fuzz");
  g_signal_connect(GTK_OBJECT(command_sel), "clicked",
                   G_CALLBACK(select_binary), "command");
  gtk_fixed_put(GTK_FIXED(opters), command_sel, 50, 50);
  gtk_widget_show(command_sel);
  /* A toggle for turning buffer size 0 on and off */
  buf_size_zero = gtk_toggle_button_new_with_label("Buffer Size 0");
  g_signal_connect(GTK_OBJECT(buf_size_zero), "clicked",
                   G_CALLBACK(set_buffer_size), "buf_size_zero");
  gtk_fixed_put(GTK_FIXED(opters), buf_size_zero, 50, 200);
  gtk_widget_show(buf_size_zero);
  /* Show that part of the screen */
  gtk_widget_show(opters);
  /* Show the whole window at once */
  gtk_widget_show_all(window);
  gtk_main();
  return(0);
}
