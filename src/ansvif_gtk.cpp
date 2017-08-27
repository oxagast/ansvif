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

#include <cstring>
#include <gtk/gtk.h>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>

FILE *popen2(std::string command, std::string type, int &pid,
             std::string low_lvl_user);
int pclose2(FILE *fp, pid_t pid);

std::string buffer_size = " -b 32 ";
std::string template_file;
std::string binary_file;
std::string ansvif_call;
GtkWidget *caller_box;
GtkTextBuffer *buffer;
GtkTextIter iter;
GtkWidget *template_sel_t;
GtkWidget *command_sel_t;

static void enter_callback(GtkWidget *widget, GtkWidget *caller_box) {
  const gchar *entry_text;
  entry_text = gtk_entry_get_text(GTK_ENTRY(caller_box));
  // printf ("Entry contents: %s\n", entry_text);
  // printf("%s", ansvif_call.c_str());
}

std::string ansvif_str() {
  ansvif_call = "./ansvif " + buffer_size + binary_file + template_file;
  return (ansvif_call);
}

int fuzz_call(GtkTextBuffer *buffe) {
  /* put together the call to ansvif */
  int com_pid;
  FILE *fp = popen2(ansvif_str(), "r", com_pid, "nobody");
  char command_out[4096] = {0};
  std::stringstream output;
  while (read(fileno(fp), command_out, sizeof(command_out) - 1) != 0) {
    output << std::string(command_out);
    memset(&command_out, 0, sizeof(command_out));
  }
  gtk_text_buffer_get_iter_at_offset(buffer, &iter, 0);
  gtk_text_buffer_set_text(buffer, output.str().c_str(), -1);
  pclose2(fp, com_pid);
  return (0);
}

static void template_selected(GtkWidget *w, GtkFileSelection *fs) {
  template_file.assign(gtk_file_selection_get_filename(GTK_FILE_SELECTION(fs)));
  template_file = " -t " + template_file;
  gtk_entry_set_text(GTK_ENTRY(caller_box), ansvif_str().c_str());
  gtk_entry_set_text(GTK_ENTRY(template_sel_t),
                     gtk_file_selection_get_filename(GTK_FILE_SELECTION(fs)));
}

static void binary_selected(GtkWidget *w, GtkFileSelection *fs) {
  binary_file.assign(gtk_file_selection_get_filename(GTK_FILE_SELECTION(fs)));
  binary_file = " -c " + binary_file;
  gtk_entry_set_text(GTK_ENTRY(caller_box), ansvif_str().c_str());
  gtk_entry_set_text(GTK_ENTRY(command_sel_t),
                     gtk_file_selection_get_filename(GTK_FILE_SELECTION(fs)));
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
                                  "flag_chars.txt");
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
    gtk_entry_set_text(GTK_ENTRY(caller_box), ansvif_str().c_str());
  } else {
    buffer_size = " -b 32 ";
    gtk_entry_set_text(GTK_ENTRY(caller_box), ansvif_str().c_str());
  }
  return (0);
}

/*
static void insert_text(GtkTextBuffer *buffer) {
  gtk_text_buffer_get_iter_at_offset(buffer, &iter, 0);
  gtk_text_buffer_insert(buffer, &iter, "ansvif", -1);
}
*/

/* Create a scrolled text area that displays a "message" */
static GtkWidget *create_text(void) {
  GtkWidget *scrolled_window;
  GtkWidget *view;

  view = gtk_text_view_new();
  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));

  scrolled_window = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                 GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

  gtk_container_add(GTK_CONTAINER(scrolled_window), view);
  fuzz_call(buffer);

  gtk_widget_show_all(scrolled_window);

  return scrolled_window;
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
  GtkWidget *ansvif_out;
  GtkWidget *text;
  gint tmp_pos;
  /* Pull in the args for gtk */
  gtk_init(&argc, &argv);
  /* Create gtk window */
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_widget_set_size_request(GTK_WIDGET(window), 850, 700);
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
  gtk_fixed_put(GTK_FIXED(opters), fuzz_it, 700, 10);
  gtk_widget_show(fuzz_it);
  /* A text box where the ansvif command goes */
  caller_box = gtk_entry_new();
  gtk_entry_set_max_length(GTK_ENTRY(caller_box), 1024);
  gtk_widget_set_size_request(GTK_WIDGET(caller_box), 650, 25);
  gtk_editable_set_editable(GTK_EDITABLE(caller_box), FALSE);
  g_signal_connect(caller_box, "activate", G_CALLBACK(enter_callback),
                   caller_box);
  gtk_entry_set_text(GTK_ENTRY(caller_box), "ansvif");
  tmp_pos = GTK_ENTRY(caller_box)->text_length;
  gtk_fixed_put(GTK_FIXED(opters), caller_box, 30, 10);
  gtk_widget_show(caller_box);
  /*  Make our template file selection */
  template_sel = gtk_button_new_with_label("Select Template");
  g_signal_connect(GTK_OBJECT(template_sel), "clicked",
                   G_CALLBACK(select_template), "template_sel");
  gtk_fixed_put(GTK_FIXED(opters), template_sel, 30, 80);
  gtk_widget_show(template_sel);
  template_sel_t = gtk_entry_new();
  gtk_entry_set_max_length(GTK_ENTRY(template_sel_t), 128);
  gtk_widget_set_size_request(GTK_WIDGET(template_sel_t), 350, 25);
  gtk_editable_set_editable(GTK_EDITABLE(template_sel_t), FALSE);
  g_signal_connect(template_sel_t, "activate", G_CALLBACK(enter_callback),
                   template_sel_t);
  tmp_pos = GTK_ENTRY(template_sel_t)->text_length;
  gtk_fixed_put(GTK_FIXED(opters), template_sel_t, 200, 80);
  gtk_widget_show(template_sel_t);
  /* Make our binary to be fuzzed file selection */
  command_sel = gtk_button_new_with_label("Select binary to fuzz");
  g_signal_connect(GTK_OBJECT(command_sel), "clicked",
                   G_CALLBACK(select_binary), "command");
  gtk_fixed_put(GTK_FIXED(opters), command_sel, 30, 50);
  gtk_widget_show(command_sel);
  command_sel_t = gtk_entry_new();
  gtk_entry_set_max_length(GTK_ENTRY(command_sel_t), 128);
  gtk_widget_set_size_request(GTK_WIDGET(command_sel_t), 350, 25);
  gtk_editable_set_editable(GTK_EDITABLE(command_sel_t), FALSE);
  g_signal_connect(command_sel_t, "activate", G_CALLBACK(enter_callback),
                   command_sel_t);
  tmp_pos = GTK_ENTRY(command_sel_t)->text_length;
  gtk_fixed_put(GTK_FIXED(opters), command_sel_t, 200, 50);
  gtk_widget_show(command_sel_t);

  /* A toggle for turning buffer size 0 on and off */
  buf_size_zero = gtk_check_button_new_with_label("Buffer Size 0");
  g_signal_connect(GTK_OBJECT(buf_size_zero), "clicked",
                   G_CALLBACK(set_buffer_size), "buf_size_zero");
  gtk_fixed_put(GTK_FIXED(opters), buf_size_zero, 50, 200);
  gtk_widget_show(buf_size_zero);

  // ansvif_out_frame = gtk_frame_new ("Left Justified Label");
  ansvif_out = gtk_label_new("ansvif output:\n");
  gtk_label_set_justify(GTK_LABEL(ansvif_out), GTK_JUSTIFY_LEFT);
  gtk_container_add(GTK_CONTAINER(opters), ansvif_out);
  gtk_fixed_put(GTK_FIXED(opters), ansvif_out, 50, 500);
  gtk_widget_show(ansvif_out);
  /* Show that part of the screen */
  text = create_text();
  gtk_fixed_put(GTK_FIXED(opters), text, 50, 540);
  gtk_widget_set_size_request(GTK_WIDGET(text), 650, 100);
  gtk_widget_show(text);
  gtk_widget_show(opters);
  /* Show the whole window at once */
  gtk_widget_show_all(window);
  gtk_main();
  return (0);
}
