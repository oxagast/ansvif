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
#include <pwd.h>
#include <sstream>
#include <string>
#include <sys/types.h>
#include <unistd.h>

FILE *popen2(std::string command, std::string type, int &pid,
             std::string low_lvl_user);
int pclose2(FILE *fp, pid_t pid);

std::string buffer_size = " -b 32 ";
std::string random_data;
std::string random_buffer_s;
std::string template_file;
std::string binary_file;
std::string ansvif_call;
std::string environment_file;
std::string maximum_args;
std::string crash_code;
GtkWidget *caller_box;
GtkTextBuffer *buffer;
GtkTextIter iter;
GtkWidget *buf_size_zero;
GtkWidget *random_buffer_size;
GtkWidget *template_sel_t;
GtkWidget *command_sel_t;
GtkWidget *environ_sel_t;
GtkWidget *bin_file;
GtkWidget *set_buf_size;
GtkWidget *templ_file;
GtkWidget *environ_file;
GtkWidget *env_file;
GtkWidget *set_exit_code;
GtkWidget *set_max_arg;

void destroy(GtkWidget *widget, gpointer *data) { gtk_main_quit(); }

std::string ansvif_str() {
  ansvif_call = "./ansvif " + random_data + random_buffer_s + buffer_size +
                binary_file + environment_file + template_file + crash_code +
                maximum_args;
  return (ansvif_call);
}

static void enter_callback(GtkWidget *widget, GtkWidget *caller_box) {
  const gchar *entry_text;
  entry_text = gtk_entry_get_text(GTK_ENTRY(caller_box));
}

static void set_buf_size_callback(GtkWidget *widget, GtkWidget *set_buf_size) {
  const gchar *buf_s;
  buf_s = gtk_entry_get_text(GTK_ENTRY(set_buf_size));
  buffer_size = " -b " + std::string(buf_s);
  gtk_entry_set_text(GTK_ENTRY(caller_box), ansvif_str().c_str());
}

static void set_exit_code_callback(GtkWidget *widget, GtkWidget *set_exit_code) {
  const gchar *e_code;
  e_code = gtk_entry_get_text(GTK_ENTRY(set_exit_code));
  crash_code = " -C " + std::string(e_code);
  gtk_entry_set_text(GTK_ENTRY(caller_box), ansvif_str().c_str());
}

static void set_max_arg_callback(GtkWidget *widget, GtkWidget *set_max_arg) {
  const gchar *max_args;
  max_args = gtk_entry_get_text(GTK_ENTRY(set_max_arg));
  maximum_args = " -M " + std::string(max_args);
  gtk_entry_set_text(GTK_ENTRY(caller_box), ansvif_str().c_str());
}

const char *get_user() {
  uid_t uid = geteuid();
  struct passwd *pw = getpwuid(uid);
  if (pw) {
    return pw->pw_name;
  }
  return "";
}

int fuzz_call(GtkTextBuffer *buffe) {
  /* put together the call to ansvif */
  int com_pid;
  FILE *fp = popen2(ansvif_str(), "r", com_pid, get_user());
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
  gtk_widget_destroy(templ_file);
}

static void env_selected(GtkWidget *w, GtkFileSelection *fs) {
  environment_file.assign(
      gtk_file_selection_get_filename(GTK_FILE_SELECTION(fs)));
  environment_file = " -e " + environment_file;
  gtk_entry_set_text(GTK_ENTRY(caller_box), ansvif_str().c_str());
  gtk_entry_set_text(GTK_ENTRY(environ_sel_t),
                     gtk_file_selection_get_filename(GTK_FILE_SELECTION(fs)));
  gtk_widget_destroy(environ_file);
}

static void binary_selected(GtkWidget *w, GtkFileSelection *fs) {
  binary_file.assign(gtk_file_selection_get_filename(GTK_FILE_SELECTION(fs)));
  binary_file = " -c " + binary_file;
  gtk_entry_set_text(GTK_ENTRY(caller_box), ansvif_str().c_str());
  gtk_entry_set_text(GTK_ENTRY(command_sel_t),
                     gtk_file_selection_get_filename(GTK_FILE_SELECTION(fs)));
  gtk_widget_destroy(bin_file);
}

int select_template() {
  templ_file = gtk_file_selection_new("Template File selection");
  g_signal_connect(templ_file, "destroy", G_CALLBACK(destroy), &templ_file);
  g_signal_connect(GTK_FILE_SELECTION(templ_file)->ok_button, "clicked",
                   G_CALLBACK(template_selected), (gpointer)templ_file);
  g_signal_connect_swapped(GTK_FILE_SELECTION(templ_file)->cancel_button,
                           "clicked", G_CALLBACK(gtk_widget_destroy),
                           templ_file);
  gtk_file_selection_set_filename(GTK_FILE_SELECTION(templ_file),
                                  "flag_chars.txt");
  gtk_widget_show(templ_file);
  gtk_main();
  return (0);
}

int select_env() {
  environ_file = gtk_file_selection_new("Template File selection");
  g_signal_connect(environ_file, "destroy", G_CALLBACK(destroy), &environ_file);
  g_signal_connect(GTK_FILE_SELECTION(environ_file)->ok_button, "clicked",
                   G_CALLBACK(env_selected), (gpointer(environ_file)));
  g_signal_connect_swapped(GTK_FILE_SELECTION(environ_file)->cancel_button,
                           "clicked", G_CALLBACK(gtk_widget_destroy),
                           environ_file);
  gtk_widget_show(environ_file);
  gtk_main();
  return (0);
}

int select_binary() {
  bin_file = gtk_file_selection_new("Binary File selection");
  g_signal_connect(bin_file, "destroy", G_CALLBACK(gtk_main_quit), NULL);
  g_signal_connect(GTK_FILE_SELECTION(bin_file)->ok_button, "clicked",
                   G_CALLBACK(binary_selected), (gpointer)bin_file);
  g_signal_connect_swapped(GTK_FILE_SELECTION(bin_file)->cancel_button,
                           "clicked", G_CALLBACK(gtk_widget_destroy), bin_file);
  gtk_file_selection_set_filename(GTK_FILE_SELECTION(environ_file), "");
  gtk_widget_show(bin_file);
  gtk_main();
  return (0);
}

int set_buffer_size(GtkWidget *buf_size_zero, gpointer data) {
  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(buf_size_zero))) {
    buffer_size = " -y ";
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(random_buffer_size), FALSE);
    gtk_entry_set_text(GTK_ENTRY(set_buf_size), "0");
    gtk_entry_set_text(GTK_ENTRY(caller_box), ansvif_str().c_str());
  } else {
    buffer_size = " -b 32 ";
    gtk_entry_set_text(GTK_ENTRY(caller_box), ansvif_str().c_str());
  }
  return (0);
}

int set_random_data(GtkWidget *random_data_only, gpointer data) {
  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(random_data_only))) {
    random_data = " -r ";
    gtk_entry_set_text(GTK_ENTRY(caller_box), ansvif_str().c_str());
  } else {
    random_data = "";
    gtk_entry_set_text(GTK_ENTRY(caller_box), ansvif_str().c_str());
  }
  return (0);
}

int set_random_size(GtkWidget *random_buffer_size, gpointer data) {
  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(random_buffer_size))) {
    random_buffer_s = " -z ";
    gtk_entry_set_text(GTK_ENTRY(caller_box), ansvif_str().c_str());
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(buf_size_zero), FALSE);
  } else {
    random_buffer_s = "";
    gtk_entry_set_text(GTK_ENTRY(caller_box), ansvif_str().c_str());
  }
  return (0);
}

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

  return (scrolled_window);
}

// void destroy(GtkWidget *widget, gpointer data) { gtk_main_quit(); }

int main(int argc, char *argv[]) {
  /* Declare some vars */
  std::string command;
  GtkWidget *window;
  GtkWidget *opters;
  GtkWidget *fuzz_it;
  GtkWidget *command_sel;
  GtkWidget *environ_sel;
  GtkWidget *template_sel;
  GtkWidget *ansvif_out;
  GtkWidget *text;
  GtkWidget *random_data_only;
  GtkWidget *b_size_label;
  GtkWidget *exit_code_label;
  GtkWidget *max_arg_label;
  gint tmp_pos;
  random_buffer_size = gtk_check_button_new_with_label("Random Buffer Size");
  buf_size_zero = gtk_check_button_new_with_label("Buffer Size 0");
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
  /* A text box where we enter buffer size */
  set_buf_size = gtk_entry_new();
  gtk_entry_set_max_length(GTK_ENTRY(set_buf_size), 4);
  gtk_widget_set_size_request(GTK_WIDGET(set_buf_size), 50, 25);
  g_signal_connect(set_buf_size, "activate", G_CALLBACK(set_buf_size_callback),
                   set_buf_size);
  gtk_entry_set_text(GTK_ENTRY(set_buf_size), "32");
  tmp_pos = GTK_ENTRY(set_buf_size)->text_length;
  gtk_fixed_put(GTK_FIXED(opters), set_buf_size, 630, 50);
  b_size_label = gtk_label_new("Buffer Size:\n");
  gtk_label_set_justify(GTK_LABEL(b_size_label), GTK_JUSTIFY_LEFT);
  gtk_container_add(GTK_CONTAINER(opters), b_size_label);
  gtk_fixed_put(GTK_FIXED(opters), b_size_label, 540, 55);
  gtk_widget_show(b_size_label);
  gtk_widget_show(set_buf_size);
/* A text box where we enter the custom crashcode */
  set_exit_code = gtk_entry_new();
  gtk_entry_set_max_length(GTK_ENTRY(set_exit_code), 3);
  gtk_widget_set_size_request(GTK_WIDGET(set_exit_code), 50, 25);
  g_signal_connect(set_exit_code, "activate", G_CALLBACK(set_exit_code_callback),
                   set_exit_code);
  gtk_entry_set_text(GTK_ENTRY(set_exit_code), "136");
  tmp_pos = GTK_ENTRY(set_exit_code)->text_length;
  gtk_fixed_put(GTK_FIXED(opters), set_exit_code, 630, 80);
  exit_code_label = gtk_label_new("Exit Code:\n");
  gtk_label_set_justify(GTK_LABEL(exit_code_label), GTK_JUSTIFY_LEFT);
  gtk_container_add(GTK_CONTAINER(opters), exit_code_label);
  gtk_fixed_put(GTK_FIXED(opters), exit_code_label, 540, 85);
  gtk_widget_show(exit_code_label);
  gtk_widget_show(set_exit_code);
/* Maximum arguments to be passed in the fuzz */
  set_max_arg = gtk_entry_new();
  gtk_entry_set_max_length(GTK_ENTRY(set_max_arg), 2);
  gtk_widget_set_size_request(GTK_WIDGET(set_max_arg), 50, 25);
  g_signal_connect(set_max_arg, "activate", G_CALLBACK(set_max_arg_callback),
                   set_max_arg);
  gtk_entry_set_text(GTK_ENTRY(set_max_arg), "4");
  tmp_pos = GTK_ENTRY(set_max_arg)->text_length; 
  gtk_fixed_put(GTK_FIXED(opters), set_max_arg, 630, 110);
  max_arg_label = gtk_label_new("Max Args:\n");
  gtk_label_set_justify(GTK_LABEL(max_arg_label), GTK_JUSTIFY_LEFT);
  gtk_container_add(GTK_CONTAINER(opters), max_arg_label);
  gtk_fixed_put(GTK_FIXED(opters), max_arg_label, 540, 115);
  gtk_widget_show(max_arg_label);
  gtk_widget_show(set_max_arg);
  /*  Make our template file selection */
  template_sel = gtk_button_new_with_label("Select Template");
  g_signal_connect(GTK_OBJECT(template_sel), "clicked",
                   G_CALLBACK(select_template), "template_sel");
  gtk_fixed_put(GTK_FIXED(opters), template_sel, 30, 80);
  gtk_widget_show(template_sel);
  template_sel_t = gtk_entry_new();
  gtk_entry_set_max_length(GTK_ENTRY(template_sel_t), 128);
  gtk_widget_set_size_request(GTK_WIDGET(template_sel_t), 330, 25);
  gtk_editable_set_editable(GTK_EDITABLE(template_sel_t), FALSE);
  g_signal_connect(template_sel_t, "activate", G_CALLBACK(enter_callback),
                   template_sel_t);
  tmp_pos = GTK_ENTRY(template_sel_t)->text_length;
  gtk_fixed_put(GTK_FIXED(opters), template_sel_t, 200, 80);
  gtk_widget_show(template_sel_t);
  /* Make our binary to be fuzzed file selection */
  command_sel = gtk_button_new_with_label("Select Binary to Fuzz");
  g_signal_connect(GTK_OBJECT(command_sel), "clicked",
                   G_CALLBACK(select_binary), "command");
  gtk_fixed_put(GTK_FIXED(opters), command_sel, 30, 50);
  gtk_widget_show(command_sel);
  command_sel_t = gtk_entry_new();
  gtk_entry_set_max_length(GTK_ENTRY(command_sel_t), 128);
  gtk_widget_set_size_request(GTK_WIDGET(command_sel_t), 330, 25);
  gtk_editable_set_editable(GTK_EDITABLE(command_sel_t), FALSE);
  g_signal_connect(command_sel_t, "activate", G_CALLBACK(enter_callback),
                   command_sel_t);
  tmp_pos = GTK_ENTRY(command_sel_t)->text_length;
  gtk_fixed_put(GTK_FIXED(opters), command_sel_t, 200, 50);
  gtk_widget_show(command_sel_t);
  /* Make our fuzz environment */
  environ_sel = gtk_button_new_with_label("Select Environment");
  g_signal_connect(GTK_OBJECT(environ_sel), "clicked", G_CALLBACK(select_env),
                   "environ_sel");
  gtk_fixed_put(GTK_FIXED(opters), environ_sel, 30, 110);
  gtk_widget_show(environ_sel);
  environ_sel_t = gtk_entry_new();
  gtk_entry_set_max_length(GTK_ENTRY(environ_sel_t), 128);
  gtk_widget_set_size_request(GTK_WIDGET(environ_sel_t), 330, 25);
  g_signal_connect(environ_sel_t, "activate", G_CALLBACK(enter_callback),
                   environ_sel_t);
  tmp_pos = GTK_ENTRY(environ_sel_t)->text_length;
  gtk_fixed_put(GTK_FIXED(opters), environ_sel_t, 200, 110);
  gtk_widget_show(environ_sel_t);
  /* A toggle for turning buffer size 0 on and off */
  g_signal_connect(GTK_OBJECT(buf_size_zero), "clicked",
                   G_CALLBACK(set_buffer_size), "buf_size_zero");
  gtk_fixed_put(GTK_FIXED(opters), buf_size_zero, 30, 200);
  gtk_widget_show(buf_size_zero);
  /* A toggle for turning all random data on and off */
  random_data_only = gtk_check_button_new_with_label("Random Data Only");
  g_signal_connect(GTK_OBJECT(random_data_only), "clicked",
                   G_CALLBACK(set_random_data), "random_data_only");
  gtk_fixed_put(GTK_FIXED(opters), random_data_only, 30, 220);
  gtk_widget_show(random_data_only);
  /* A toggle for turning buffer random buffer size on and off */
  g_signal_connect(GTK_OBJECT(random_buffer_size), "clicked",
                   G_CALLBACK(set_random_size), "random_buffer_size");
  gtk_fixed_put(GTK_FIXED(opters), random_buffer_size, 30, 180);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(random_buffer_size), TRUE);
  gtk_widget_show(random_buffer_size);
  /* ansvif output goes here */
  ansvif_out = gtk_label_new("ansvif output:\n");
  gtk_label_set_justify(GTK_LABEL(ansvif_out), GTK_JUSTIFY_LEFT);
  gtk_container_add(GTK_CONTAINER(opters), ansvif_out);
  gtk_fixed_put(GTK_FIXED(opters), ansvif_out, 50, 500);
  gtk_widget_show(ansvif_out);
  text = create_text();
  gtk_fixed_put(GTK_FIXED(opters), text, 50, 540);
  gtk_widget_set_size_request(GTK_WIDGET(text), 730, 100);
  /* Show that part of the screen */
  gtk_widget_show(text);
  gtk_widget_show(opters);
  /* Show the whole window at once */
  gtk_widget_show_all(window);
  gtk_main();
  return (0);
}
