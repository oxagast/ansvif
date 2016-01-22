#include <thread>
#include <sys/wait.h>
#include <sys/stat.h>
#include <vector>
#include <unistd.h>
#include <cstdlib>
#include <regex>
#include <iostream>
#include <stdio.h>
#include <fstream>

int toint(std::string ints);
void help_me(std::string mr_me);
std::vector<std::string> get_flags_man(char *cmd, std::string man_loc,
                                       bool verbose, bool debug,
                                       bool dump_opts);
bool match_seg(int buf_size, std::vector<std::string> opts,
               std::vector<std::string> spec_env, std::string path_str,
               std::string strip_shell, bool rand_all, bool write_to_file,
               std::string write_file_n, bool rand_buf,
               std::vector<std::string> opt_other, bool is_other,
               std::string other_sep, int t_timeout, std::string low_lvl_user,
               std::string junk_file_of_args, std::string always_arg_before,
               std::string always_arg_after, bool never_rand,
               std::string run_command, std::regex sf_reg, bool verbose,
               bool debug);
std::vector<std::string> get_flags_template(std::string filename, bool verbose,
                                            bool debug);
std::vector<std::string> get_other(std::string filename, bool verbose,
                                   bool debug);

int main(int argc, char *argv[]) {
  char *man_chr;
  int opt;
  int num_threads = 2;
  int t_timeout = 3;
  std::vector<std::string> opts;
  std::vector<std::string> spec_env;
  std::vector<std::string> opt_other;
  std::string man_loc = "8";
  std::string buf_size;
  std::string mp;
  std::string template_file;
  std::string strip_shell = "`<>\n|&\[]\()\{}:;";
  std::string u_strip_shell;
  std::string write_file_n = "";
  std::string path_str = "";
  std::string other_sep = "";
  std::string low_lvl_user = "nobody";
  std::string junk_file_of_args;
  std::string always_arg = "";
  std::string run_command = "";
  std::regex sf_reg("(-1073741819|-1073740791|-1073741571|-532459699)");
  bool template_opt = false;
  bool man_opt = false;
  bool rand_all = false;
  bool rand_buf = false;
  bool write_to_file = false;
  bool u_strip_shell_set = false;
  bool verbose = false;
  bool debug = false;
  bool is_other = false;
  bool dump_opts = false;
  bool never_rand = false;
  while ((opt = getopt(argc, argv,
                       "m:p:t:e:c:f:o:b:s:x:R:A:F:S:L:W:B:C:hrzvdDn")) != -1) {
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
        man_chr = optarg;
        break;
      case 'f':
        num_threads = std::atoi(optarg);
        break;
      case 'o':
        write_to_file = true;
        write_file_n = optarg;
        break;
      case 'h':
        help_me(argv[0]);
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
        sf_reg = optarg;
        break;
      default:
        help_me(argv[0]);
    }
  }
  if (u_strip_shell_set == true) {
    strip_shell = u_strip_shell + ">\n";
  }
  if ((man_opt == true) && (template_opt == false)) {
    opts = get_flags_man(man_chr, man_loc, verbose, debug, dump_opts);
  } else if ((man_opt == false) && (template_opt == true)) {
    opts = get_flags_template(template_file, verbose, debug);
  } else if ((man_opt == true) && (template_opt == true)) {
    help_me(argv[0]);
  } else if ((man_opt == false) && (template_opt == false)) {
    help_me(argv[0]);
  } else if (path_str == "") {
    help_me(argv[0]);
  } else {
    help_me(argv[0]);
  }
  std::istringstream b_size(buf_size);
  int is_int;
  if (!(b_size >> is_int)) {
    help_me(argv[0]);
  }
  char buf_char_maybe;
  if (b_size >> buf_char_maybe) {
    help_me(argv[0]);
  } else {
    int buf_size_int = toint(buf_size);
    std::vector<std::thread> threads;
    bool did_it_fault;
    for (int cur_thread = 1; cur_thread <= num_threads; ++cur_thread)
      threads.push_back(std::thread(
          match_seg, buf_size_int, opts, spec_env, path_str, strip_shell,
          rand_all, write_to_file, write_file_n, rand_buf, opt_other, is_other,
          other_sep, t_timeout, low_lvl_user, junk_file_of_args,
          always_arg_before, always_arg_after, never_rand, run_command, sf_reg,
          verbose, debug));  // Thrift Shop
    for (auto &all_thread : threads)
      all_thread.join();  // is that your grandma's coat?
    exit(0);
  }
}
