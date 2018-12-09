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
#include <signal.h>
#include <stdio.h>
#include <sys/stat.h>
#ifdef __unix__
#include <sys/wait.h>
#endif
#ifdef __ANDROID__
#include "version.h"
#endif
#ifdef __NOTANDROID__
#include "src/version.h"
#endif
#ifdef _WIN32
#include "version.h"
#endif
#include <sys/types.h>
#include <thread>
#include <unistd.h>
#include <vector>

int toint(std::string ints, std::string my_prog);
void help_me(std::string mr_me);
bool file_exists(const std::string &filen);
std::vector<std::string> get_flags_man(std::string man_page,
                                       std::string man_loc, bool verbose,
                                       bool debug, bool dump_opts);

struct Options {
public:
  int static_args;
  std::vector<std::string> opts;
  std::vector<std::string> spec_env;
  std::vector<std::string> opt_other;
  std::string t_timeout;
  std::string strip_shell;
  std::string u_strip_shell;
  std::string write_file_n;
  std::string path_str;
  std::string other_sep;
  std::string low_lvl_user;
  std::string junk_file_of_args;
  std::string always_arg_before;
  std::string always_arg_after;
  std::string fault_code;
  bool rand_all;
  bool verbose;
  bool debug;
  bool dump_opts;
  bool never_rand;
  bool valgrind;
  bool percent_sign;
} options;

struct BuffCont {
  public:
  int buf_size_int;
  bool rand_buf;
} buffercontrol;

struct RunCommands {
  public:
  std::string run_command;
  std::string before_command;
  std::string prog_name;
} runcoms;

struct Monopoly {
  public:
  bool keep_going;
  bool single_try;
} dontpassgo;

void match_seg(Options, RunCommands, Monopoly, BuffCont);
std::vector<std::string> get_flags_template(std::string filename, bool verbose,
                                            bool debug);
std::vector<std::string> get_other(std::string filename, bool verbose,
                                   bool debug);
void write_seg(std::string filename, std::string seg_line);

/* globals to be passed to sig_handler because its easier this way */
std::string write_file_n, junk_file_of_args;
std::string ver = "1.11"; /* ansvif version */
void sig_handler(int sig) {
  /* flush the screen buffer  then sleep before printing
   * the message about killing threads
   */
  std::cout.flush();
  sleep(1);
  std::cout << std::endl << "Cought ctrl+c, Killing threads..." << std::endl;
  std::cout << "Cleaning up..." << std::endl;
  /* append the crash logs to their respective strings so that we
   * can clean up after ourselves if there was no crash
   */
  std::string crash_file = write_file_n + ".crash.ansvif.log";
  std::string output_file = write_file_n + ".output.ansvif.log";
  /* clean up the junk files */
  unlink(junk_file_of_args.c_str());
  unlink(crash_file.c_str());
  unlink(output_file.c_str());
  /* exit our program as the user wanted to */
  std::cout << "Exiting cleanly..." << std::endl;
  exit(0);
}

void version() {
  std::cout << "ansvif v" << ver << " -- A Not So Very Intelligent Fuzzer"
            << std::endl;
  exit(0);
}

int main(int argc, char *argv[]) { // initialize our main
  /* initialize all our variables for startup! */
  /* how many options? */
  int opt;
  int thread_count_def = 2;
  /* int thread_timeout_def = 3; */

  Options options = {
    static_args : 4,
    opts : {},
    spec_env : {},
    opt_other : {},
    t_timeout : "3",
#ifdef __NOTANDROID__
    strip_shell : "\"`<>\n|&\[]\()\{}:;$'",
#endif
#ifdef _WIN32
    strip_shell : "[]:|<>+;=.?\n\r\\0",
#endif
#ifdef __ANDROID__
    strip_shell : "\"`<>\n|&\[]\()\{}:;$'",
#endif
    u_strip_shell : "",
    write_file_n : "",
    path_str : "",
    other_sep : "",
    low_lvl_user : "nobody",
    junk_file_of_args : "",
    always_arg_before : "",
    always_arg_after : "",
#ifdef __NOTANDROID__
    fault_code : "134",
#endif
#ifdef __ANDROID__
    fault_code : "134",
#endif
#ifdef _WIN32
    fault_code : "-1073741819",
#endif
    rand_all : false,
    verbose : false,
    debug : false,
    dump_opts : false,
    never_rand : false,
    valgrind : false,
    percent_sign : false,
  };

  BuffCont buffercontrol {
    buf_size_int : -1,
    rand_buf : false
  };

  Monopoly dontpassgo {
    keep_going : false,
    single_try : false
  };

  RunCommands runcoms {
    run_command : "",
    before_command : "",
    prog_name : ""
  };

  /* first off we're going to start the signal handler incase they
   * do ctrl+c or something
   */
  signal(SIGINT, sig_handler);
  std::string buf_size, template_file, man_loc, man_page, num_threads;
  /* now we can start grabbing all the options! */
  while ((opt = getopt(
              argc, argv,
              "m:p:t:e:c:f:o:b:s:x:R:A:F:E:S:L:W:B:M:C:N:y1hrzvdDnVPKi0")) !=
         -1) {
    switch (opt) {
    case 'v':
      options.verbose = true;
      break;
    case 'd':
      options.debug = true;
      break;
    case 't':
      template_file = optarg;
      break;
    case 'c':
      options.path_str = optarg;
      break;
    case 'b':
      buf_size = optarg;
      break;
    case 'e':
      options.spec_env =
          get_flags_template(optarg, options.verbose, options.debug);
      break;
    case 'p':
      man_loc = optarg;
      break;
    case 'm':
      man_page = optarg;
      break;
    case 'f':
      num_threads = optarg;
      break;
    case 'o':
      options.write_file_n = optarg;
      break;
    case 'h':
      help_me(argv[0]);
      break;
    case 'r':
      options.rand_all = true;
      break;
    case 'z':
      buffercontrol.rand_buf = true;
      break;
    case 's':
      options.u_strip_shell = optarg;
      break;
    case 'x':
      options.opt_other = get_other(optarg, options.verbose, options.debug);
      break;
    case 'D':
      options.dump_opts = true;
      break;
    case 'S':
      options.other_sep = optarg;
      break;
    case 'L':
      options.low_lvl_user = optarg;
      break;
    case 'F':
      options.junk_file_of_args = optarg;
      break;
    case 'A':
      options.always_arg_after = optarg;
      break;
    case 'B':
      options.always_arg_before = optarg;
      break;
    case 'n':
      options.never_rand = true;
      break;
    case 'R':
      runcoms.run_command = optarg;
      break;
    case 'W':
      options.t_timeout = optarg;
      break;
    case 'C':
      options.fault_code = optarg;
      break;
    case 'V':
      options.valgrind = true;
      break;
    case '1':
      dontpassgo.single_try = true;
      break;
    case 'P':
      options.percent_sign = true;
      break;
    case 'M':
      options.static_args = toint(optarg, argv[0]);
      break;
    case 'y':
      buffercontrol.buf_size_int = 0;
      break;
    case 'K':
      dontpassgo.keep_going = true;
      break;
    case 'E':
      runcoms.before_command = optarg;
      break;
    case 'i':
      version();
      break;
    case '0':
      options.strip_shell = options.strip_shell + "\\x00";
      break;
    case 'N':
      runcoms.prog_name = optarg;
      break;
    default:
      help_me(argv[0]);
    }
  }
#ifdef __ANDROID__
  if (file_exists("/sdcard/ansvif/crashed") == true) {
    unlink("/sdcard/ansvif/crashed");
  }
#endif
#ifdef __NOTANDROID__
  if (file_exists("/tmp/a.crashed") == true) {
    unlink("/tmp/a.crashed");
  }
#endif
  /* always strip a new line no matter what the user says */
  options.strip_shell = options.u_strip_shell + ">\n";
  if ((man_page != "") && (template_file == "")) {
    /* because we're getting stuff from the manpage */
    options.opts =
        get_flags_man(man_page, man_loc, options.verbose,
                      options.debug, options.dump_opts);
  } else if ((man_page == "") && (template_file != "")) {
    /* we're getting stuff from a template */
    options.opts = get_flags_template(template_file, options.verbose,
                                      options.debug);
  } else if ((man_page != "") && (template_file != "")) {
    /* send them to help because you can't have a manpage and
     * a template at the same time
     */
    std::cerr << "Don't specifiy a manpage and template at the same time"
              << std::endl;
    help_me(argv[0]);
  } else if ((man_page == "") && (template_file == "")) {
    /* can't fuzz if we don't have a template or manpage as
     * a starting point, if you want nothing just touch a file
     * and use it as a template
     */
    std::cerr << "You didn't specy a manpage or template." << std::endl;
    help_me(argv[0]);
  }
  if (file_exists(options.path_str) == false) {
    /* if they didn't specify a command path then error out */
    std::cerr << "No command at path to fuzz..." << std::endl;
    help_me(argv[0]);
  }
  if ((options.junk_file_of_args != "") && (options.opt_other.size() != 0)) {
    /* this will fix the -F no -x bug */
    help_me(argv[0]);
  }
  /* make sure the buffer size is really an integar and if it
   * happens to not be, then we'll send them to the help page,
   * otherwise we'll turn it into type int
   */
  if ((buffercontrol.buf_size_int == 0) && (buf_size != "")) {
    help_me(argv[0]);
  }
  if ((buffercontrol.buf_size_int == -1) && (buf_size != "")) {
    buffercontrol.buf_size_int = toint(buf_size, argv[0]);
  }
  if (buffercontrol.buf_size_int == -1) {
      	  help_me(argv[0]);
  }
  if (buffercontrol.buf_size_int < 0) {
    std::cerr << "Buffer must be a positive integer." << std::endl;
    help_me(argv[0]);
  }
  /* make sure the thread count is an integar the same way
   * we did with the buffer size
   * a
   */
  int thread_count_int = thread_count_def;
  if(num_threads != "") {
  thread_count_int = toint(num_threads, argv[0]);
  }
  /* int thread_timeout_int = thread_timeout_def; */
  /* thread_timeout_int = toint(options.t_timeout, argv[0]); */
  /* if we're not doing a single try then turn on
   * threading
   */
  if (dontpassgo.single_try == false) {
    /* initialize threading! */
    std::vector<std::thread> threads;
    for (int cur_thread = 1; cur_thread <= thread_count_int; ++cur_thread)
      threads.push_back(std::thread(match_seg, options, runcoms, dontpassgo, buffercontrol));
    /* thrift shop */
    for (auto &all_thread : threads)
      all_thread.join();
    /* is that your grandma's coat??? */
  }
  if (dontpassgo.single_try == true) {
    /* no threads or anything since we're only doing a
     * single run
     */
    match_seg(options, runcoms, dontpassgo, buffercontrol);
  }
  /* exit cleanly! */
  exit(0);
}
