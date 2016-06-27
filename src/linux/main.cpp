/*
 * ansvif
 * A Not So Very Intelligent Fuzzer
 * Marshall Whittaker / oxagast
 */

//    __ _  _  __   ___  __  ____ ____ 
//   /  ( \/ )/ _\ / __)/ _\/ ___(_  _)
//  (  O )  (/    ( (_ /    \___ \ )(  
//   \__(_/\_\_/\_/\___\_/\_(____/(__)

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <regex>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <thread>
#include <unistd.h>
#include <vector>

int toint(std::string ints);
void help_me(std::string mr_me, std::string ver);
std::vector<std::string> get_flags_man(std::string man_page,
                                       std::string man_loc, bool verbose,
                                       bool debug, bool dump_opts);
bool match_seg(int buf_size, std::vector<std::string> opts,
               std::vector<std::string> spec_env, std::string path_str,
               std::string strip_shell, bool rand_all, bool write_to_file,
               std::string write_file_n, bool rand_buf,
               std::vector<std::string> opt_other, bool is_other,
               std::string other_sep, int t_timeout, std::string low_lvl_user,
               std::string junk_file_of_args, std::string always_arg_before,
               std::string always_arg_after, bool never_rand,
               std::string run_command, std::string fault_code, bool valgrind,
               bool single_try, bool percent_sign, bool verbose, bool debug,
               std::string ver);
std::vector<std::string> get_flags_template(std::string filename, bool verbose,
                                            bool debug);
std::vector<std::string> get_other(std::string filename, bool verbose,
                                   bool debug);
void write_seg(std::string filename, std::string seg_line);

int main(int argc, char *argv[]) { // initialize our main
  int opt;                         // initialize opt for how many options
  int num_threads = 2;             // how many threads are we using?
  int t_timeout = 3; // what is the seconds that the thread will time out in if
                     // it takes too long?
  std::vector<std::string> opts; // the options that are pulled out of the
                                 // manpage or template file go in this vector
  std::vector<std::string>
      spec_env; // the environment variables go in this vector
  std::vector<std::string> opt_other; // opt other is to be combind with -x for
                                      // file output to be fed in or to be used
                                      // as sub options
  std::string man_loc = "8";          // the default manpage location
  std::string buf_size; // the size of the buffer we will use (this can varry,
                        // has to be an integar)
  std::string mp;       // the string that holds the manpage
  std::string template_file; // the location of the template file
  std::string strip_shell =
      "`<>\n|&\[]\()\{}:;\\'\"$"; // what characters not to use
                                  // in the command that goes
                                  // to /bin/sh
  std::string u_strip_shell; // if the user supplied extra characters to strip
                             // out, they go here
  std::string write_file_n = ""; // the log file's location and name
  std::string path_str = "";     // the path to the command
  std::string other_sep = "";    // if space isn't to be used, what is?
  std::string low_lvl_user =
      "nobody"; // the user that we want to run as if we're running su as root
  std::string junk_file_of_args = ""; // a file full of arguments to be supplied
  std::string always_arg_before = ""; // the argument to be supplied after the
                                      // command, but before the rest of the
                                      // fuzz
  std::string always_arg_after =
      "";                         // the argument to be supplied after the fuzz
  std::string run_command = "";   // the command name to be run
  std::string man_page = "";      // the man page name
  std::string fault_code = "134"; // default dummy fault code
                                  // we're going to trap, this
                                  // can be user selected
  bool template_opt = false;      // default is false
  bool man_opt = false;           // default is false
  bool rand_all = false;          // default for randomize all the fuzz is off
  bool rand_buf = false;      // default for randomizing the buffer size is off
  bool write_to_file = false; // logging is off by default
  bool u_strip_shell_set = false; // the user hasn't set a strip option yet
  bool verbose = false;           // default is not to talk alot
  bool debug = false;             // default is not to be noisey as fuckkkk
  bool is_other = false;   // a different seperator other than space isn't
                           // normally used, so its false by default
  bool dump_opts = false;  // we're not dumping options by default
  bool never_rand = false; // this is false because we want some randomization
                           // in the fuzz usually
  bool valgrind = false;   // since we don't know if valgrind is installed or if
                           // the user wants to use it it's off by default
  bool single_try = false; // this is off by default because it's usually only
                           // used with wrappers like xargs
  bool percent_sign = false; // percent sign stuff, default is off
  std::string ver = "1.5.2"; // the version
  while ((opt = getopt(
              argc, argv,
              "m:p:t:e:c:f:o:b:s:x:R:A:F:S:L:W:B:C:1hrzvdDnVP")) != // these are
                                                                    // all the
                                                                    // flags
         -1) {
    switch (opt) {
    case 'v':
      verbose = true;
      break;
    case 'd':
      debug = true;
      break;
    case 't':
      template_opt = true;
      template_file = optarg;
      break;
    case 'c':
      path_str = optarg;
      break;
    case 'b':
      buf_size = optarg;
      break;
    case 'e':
      spec_env = get_flags_template(optarg, verbose, debug);
      break;
    case 'p':
      man_loc = optarg;
      break;
    case 'm':
      man_opt = true;
      man_page = optarg;
      break;
    case 'f':
      num_threads = std::atoi(optarg);
      break;
    case 'o':
      write_to_file = true;
      write_file_n = optarg;
      break;
    case 'h':
      help_me(argv[0], ver);
      break;
    case 'r':
      rand_all = true;
      break;
    case 'z':
      rand_buf = true;
      break;
    case 's':
      u_strip_shell = optarg;
      u_strip_shell_set = true;
      break;
    case 'x':
      opt_other = get_other(optarg, verbose, debug);
      is_other = true;
      break;
    case 'D':
      dump_opts = true;
      break;
    case 'S':
      other_sep = optarg;
      break;
    case 'L':
      low_lvl_user = optarg;
      break;
    case 'F':
      junk_file_of_args = optarg;
      break;
    case 'A':
      always_arg_after = optarg;
      break;
    case 'B':
      always_arg_before = optarg;
      break;
    case 'n':
      never_rand = true;
      break;
    case 'R':
      run_command = optarg;
      break;
    case 'W':
      t_timeout = std::atoi(optarg);
      break;
    case 'C':
      fault_code = optarg;
      break;
    case 'V':
      valgrind = true;
      break;
    case '1':
      single_try = true;
      break;
    case 'P':
      percent_sign = true;
      break;
    default:
      help_me(argv[0], ver);
    }
  }
  if (u_strip_shell_set == true) {
    strip_shell = u_strip_shell +
                  ">\n"; // always strip a new line no matter what the user says
  }
  if ((man_opt == true) && (template_opt == false)) {
    opts = get_flags_man(
        man_page, man_loc, verbose, debug,
        dump_opts); // we'll be getting stuff from the manpage this time
  } else if ((man_opt == false) && (template_opt == true)) {
    opts = get_flags_template(
        template_file, verbose,
        debug); // we'll be getting stuff from a template file this time
  } else if ((man_opt == true) && (template_opt == true)) {
    help_me(argv[0],
            ver); // give the help file is they supply both the template
                  // and manpage because they are mutually exclusive
  } else if ((man_opt == false) && (template_opt == false)) {
    help_me(argv[0],
            ver); // if they supply neither a template or manpage then we
                  // can't fuzz, if you wanna fuzz blank then just fuzz
                  // with a template made with touch, or make a file and
                  // only put a space in it
  } else if (path_str == "") {
    help_me(argv[0], ver); // if they didn't specify the command, error out
  } else {
    help_me(argv[0],
            ver); // if theres any other errors uncaught then error out, or
                  // if they supplied a option thats not listed
  }
  if ((junk_file_of_args != "") && (is_other == false)) { // fixes -F no -x bug
    help_me(argv[0], ver);
  }
  std::istringstream b_size(buf_size); // we're going to now make sure the buf
                                       // size is an integer only
  int is_int;                          // declare
  if (!(b_size >> is_int)) {           // and if it's not an integar then...
    help_me(argv[0], ver);             // send them to get mental help
  }
  char buf_char_maybe;            // if it's a char in the buf size...
  if (b_size >> buf_char_maybe) { // then also...
    help_me(argv[0], ver);        // send them to the nuthouse
  } else {
    int buf_size_int = toint(buf_size); // otherwise we're going to turn the
                                        // buf size into an integar
    if (single_try ==
        false) { // if single try isn't turned on then we're going to use...
      std::vector<std::thread> threads; // DING DING DING! threading!
      for (int cur_thread = 1; cur_thread <= num_threads; ++cur_thread)
        threads.push_back(std::thread(
            match_seg, buf_size_int, opts, spec_env, path_str, strip_shell,
            rand_all, write_to_file, write_file_n, rand_buf, opt_other,
            is_other, other_sep, t_timeout, low_lvl_user, junk_file_of_args,
            always_arg_before, always_arg_after, never_rand, run_command,
            fault_code, valgrind, single_try, percent_sign, verbose, debug,
            ver)); // Thrift Shop
      for (auto &all_thread : threads)
        all_thread.join(); // is that your grandma's coat?
    }
    if (single_try == true) {
      match_seg(buf_size_int, opts, spec_env, path_str, strip_shell, rand_all,
                write_to_file, write_file_n, rand_buf, opt_other, is_other,
                other_sep, t_timeout, low_lvl_user, junk_file_of_args,
                always_arg_before, always_arg_after, never_rand, run_command,
                fault_code, valgrind, single_try, percent_sign, verbose, debug,
                ver); // if we're only doing a single run then we'll just
                      // directly call the match_seg subroutine
    }
    exit(0); // exit cleanly now.
  }
}
