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

/* globals to be passed to sig_handler because its easier this way */
std::string write_file_n, junk_file_of_args;

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

int main(int argc, char *argv[]) { // initialize our main
  /* initialize all our variables for startup! */
  /* how many options? */
  int opt, thread_count_def = 2, thread_timeout_def = 3;
  /* the options that are pulled out of the manpage or
   * template go into the opts vector, the environment
   * variables go into spec_env and the extra variables
   * that are invoked with -x go into opt_other
   */
  std::vector<std::string> opts, spec_env, opt_other;
  /* here we're initializing the variables for the thread
   * timeout, the manpage location (8 is the default because
   * thats where most manpages are stored, but 1 could also
   * work.) as a string so we can check int later, the number
   * of threads we're using, the buffer size as a string so
   * that we chan check later if it is really an integar, 
   * the string that holds the manpage, the location of the
   * template file, the characters we should strip that are
   * sh dependant (characters that would interfere with the
   * fuzz), if the user supplied any extra characters to
   * strip out of the fuzz, the log file's name, the path
   * of the command, the other seperator that should be used
   * if not space with the -x option, the lower level user
   * is initialized as 'nobody' by default if we're running
   * as root, the command to run after the fuzz (good for
   * killing off nasty processes afterwards if the reaper
   * doesn't get them), the man page name, as well as
   * a dummy fault code thats a duplicate so that the user
   * can supply a fault code to the program that it should
   * catch as well as the other default ones.
   */
  std::string t_timeout = "3", man_loc = "8", num_threads = "2",
    buf_size, mp, template_file, strip_shell = "`<>\n|&\[]\()\{}:;\\'\"$",
    u_strip_shell, write_file_n = "", path_str = "", other_sep = "", low_lvl_user =
    "nobody", junk_file_of_args = "", always_arg_before = "", always_arg_after =
    "", run_command = "", man_page = "", fault_code = "134";
  /* declare some more variables as boolean, the template_opt,
   * manpage option, if random is always used, if we're randomizing
   * the buffer size, if we're writing to a file (logging), if the
   * user has set custom stripping characters, if we're going to be
   * jabbering a lot, if we're going to print the printf stuff as we
   * fuzz, if there is another seperator besides space, if we're just
   * going to dump the manpage options, if we're never going to
   * inject random data, if we're using valgrind, if we're going to
   * do a single dry run, if we're going to need the % sign (this is
   * good for web browser fuzzing)
   */
  bool template_opt = false, man_opt = false, rand_all = false, rand_buf = false,
    write_to_file = false, u_strip_shell_set = false, verbose = false,
    debug = false, is_other = false, dump_opts = false, never_rand = false,
    valgrind = false, single_try = false, percent_sign = false;
  /* what version of ansvif are we running? */
    std::string ver = "1.6";
  /* first off we're going to start the signal handler incase they
   * do ctrl+c or something
   */
  signal(SIGINT, sig_handler);
  /* now we can start grabbing all the options! */
  while ((opt = getopt(
    argc, argv,
    "m:p:t:e:c:f:o:b:s:x:R:A:F:S:L:W:B:C:1hrzvdDnVP")) != -1) {
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
      num_threads = optarg;
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
      t_timeout = optarg;
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
    /* always strip a new line no matter what the user says */
    strip_shell = u_strip_shell + ">\n";
  }
  if ((man_opt == true) && (template_opt == false)) {
    /* because we're getting stuff from the manpage */
    opts = get_flags_man(man_page, man_loc, verbose, debug, dump_opts);
  } else if ((man_opt == false) && (template_opt == true)) {
    /* we're getting stuff from a template */
    opts = get_flags_template(template_file, verbose, debug);
  } else if ((man_opt == true) && (template_opt == true)) {
    /* send them to help because you can't have a manpage and
     * a template at the same time
     */
    help_me(argv[0], ver);
  } else if ((man_opt == false) && (template_opt == false)) {
    /* can't fuzz if we don't have a template or manpage as
     * a starting point, if you want nothing just touch a file
     * and use it as a template
     */
    help_me(argv[0], ver);
  } else if (path_str == "") {
    /* if they didn't specify a command path then error out */
    help_me(argv[0], ver);
  } else {
    /* like if the option they supplied doesn't exist... */
    help_me(argv[0], ver);
  }
  if ((junk_file_of_args != "") && (is_other == false)) {
    /* this will fix the -F no -x bug */
    help_me(argv[0], ver);
  }
  /* make sure the buffer size is really an integar and if it
   * happens to not be, then we'll send them to the help page,
   * otherwise we'll turn it into type int
   */
  std::istringstream b_size(buf_size);
  int is_int_b_s;
  if (!(b_size >> is_int_b_s)) {
    help_me(argv[0], ver);
  }
  char buf_char_maybe_b_s;
  if (b_size >> buf_char_maybe_b_s) {
    help_me(argv[0], ver);
  } else {
    int buf_size_int = toint(buf_size);
    /* make sure the thread count is an integar the same way
     * we did with the buffer size
     */
    int thread_count_int = thread_count_def;
    std::istringstream t_count(num_threads);
    int is_int_t_c;
    if (!(t_count >> is_int_t_c)) {
      help_me(argv[0], ver);
    }
    char buf_char_maybe_t_c;
    if (t_count >> buf_char_maybe_t_c) {
      help_me(argv[0], ver);
    } else {
      thread_count_int = toint(num_threads);
      int thread_timeout_int = thread_timeout_def;
      /* we're gonna do the same thing with the thread timeout */
      std::istringstream th_timeout(t_timeout);
      int is_int_t_t;
      if (!(th_timeout >> is_int_t_t)) {
        help_me(argv[0], ver);
      }
      char buf_char_maybe_t_t;
      if (th_timeout >> buf_char_maybe_t_t) {
        help_me(argv[0], ver);
      } else {
        thread_timeout_int = toint(t_timeout);
        /* if we're not doing a single try then turn on 
         * threading 
         */
        if (single_try == false) {
          /* initialize threading! */
          std::vector<std::thread> threads;
          for (int cur_thread = 1; cur_thread <= thread_count_int; ++cur_thread)
            threads.push_back(std::thread(
                match_seg, buf_size_int, opts, spec_env, path_str, strip_shell,
                rand_all, write_to_file, write_file_n, rand_buf, opt_other,
                is_other, other_sep, thread_timeout_int, low_lvl_user,
                junk_file_of_args, always_arg_before, always_arg_after,
                never_rand, run_command, fault_code, valgrind, single_try,
                percent_sign, verbose, debug,
                ver));
            /* thrift shop */
          for (auto &all_thread : threads)
            all_thread.join();
          /* is that your grandma's coat??? */
        }
        if (single_try == true) {
          /* no threads or anything since we're only doing a
           * single run
           */
          match_seg(buf_size_int, opts, spec_env, path_str, strip_shell,
                    rand_all, write_to_file, write_file_n, rand_buf, opt_other,
                    is_other, other_sep, thread_timeout_int, low_lvl_user,
                    junk_file_of_args, always_arg_before, always_arg_after,
                    never_rand, run_command, fault_code, valgrind, single_try,
                    percent_sign, verbose, debug,
                    ver);
        }
        /* exit cleanly! */
        exit(0);
      }
    }
  }
}
