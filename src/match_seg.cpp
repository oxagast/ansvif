#include <string>
#include <vector>
#include <unistd.h>
#include <iomanip>
#include <thread>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <regex>
#include <fstream>
#include <cstring>

std::string remove_chars(const std::string &source, const std::string &chars);
int reaper(int grim, int t_timeout);
FILE *popen2(std::string command, std::string type, int &pid,
             std::string low_lvl_user);
int pclose2(FILE *fp, pid_t pid);
void write_seg(std::string filename, std::string seg_line);
int rand_me_plz(int rand_from, int rand_to);
std::string make_garbage(int trash, int buf, std::string opt_other_str,
                         bool is_other, bool never_rand);
bool file_exists(const std::string &filen);
void write_junk_file(std::string filename, std::vector<std::string> opt_other,
                     int buf_size, int rand_spec_one, int rand_spec_two,
                     bool never_rand, std::string other_sep, bool verbose);
std::vector<std::string> get_out_str(std::string env_str, std::string valgrind_str, std::string sys_str,
                                     std::string path_str,
                                     std::string always_arg_after);

bool match_seg(int buf_size, std::vector<std::string> opts,
               std::vector<std::string> spec_env, std::string path_str,
               std::string strip_shell, bool rand_all, bool write_to_file,
               std::string write_file_n, bool rand_buf,
               std::vector<std::string> opt_other, bool is_other,
               std::string other_sep, int t_timeout, std::string low_lvl_user,
               std::string junk_file_of_args, std::string always_arg_before,
               std::string always_arg_after, bool never_rand,
               std::string run_command, std::regex sf_reg, bool valgrind,
               bool verbose, bool debug) {
  bool segged = false;
  std::string valgrind_str;
  if (valgrind == true) {
    valgrind_str = "/usr/bin/valgrind --leak-check=full --xml=yes --xml-file=ansvif.valgrind.log --error-exitcode=139";
  }
  if (valgrind == false) {
    valgrind_str = "";
  }
  if (file_exists(path_str) == true) {
    while (segged == false) {
      int rand_spec_one, rand_spec_two;
      if (rand_all == true) {
        rand_spec_one = 3;  // make for always random data
        rand_spec_two = 3;  // always random too
      } else {
        rand_spec_one = 0; // any data entered
        rand_spec_two = 12; // any data entered
      }
      std::vector<std::string> junk_opts_env;
      std::vector<std::string> junk_opts;
      std::string env_str;
      std::string sys_str = always_arg_before + " ";
      if (junk_file_of_args != "") {
        write_junk_file(junk_file_of_args, opt_other, buf_size, rand_spec_one,
                        rand_spec_two, never_rand, other_sep, verbose);
      }
      int sep_type;
      for (int cmd_flag_l = 0; cmd_flag_l < opts.size();
           cmd_flag_l++) {             // loop around the options
        if (rand_me_plz(0, 1) == 1) {  // roll tha die
          junk_opts.push_back(
              opts.at(cmd_flag_l));  // put the random arg in the vector
        }
      }
      for (int cmd_flag_a = 0; cmd_flag_a < spec_env.size();
           cmd_flag_a++) {             // loop around the options
        if (rand_me_plz(0, 1) == 1) {  // roll tha die
          junk_opts_env.push_back(
              spec_env.at(cmd_flag_a));  // put the random arg in the vector
        }
      }
      if (is_other == true) {
        if (rand_buf == true) {
          for (std::vector<std::string>::const_iterator junk_opt_env =
                   junk_opts_env.begin();
               junk_opt_env != junk_opts_env.end();
               ++junk_opt_env) {  // loop through the vector of junk envs
            std::string oscar_env = remove_chars(
                make_garbage(rand_me_plz(rand_spec_one, rand_spec_two),
                             rand_me_plz(1, buf_size),
                             opt_other.at(rand_me_plz(0, opt_other.size() - 1)),
                             is_other, never_rand),
                " ");
            if (oscar_env != "OOR") {
              env_str = env_str + *junk_opt_env + oscar_env + " ";
            }
          }
          for (std::vector<std::string>::const_iterator junk_opt =
                   junk_opts.begin();
               junk_opt != junk_opts.end();
               ++junk_opt) {  // loop through the vector of junk opts
            std::string oscar = remove_chars(
                make_garbage(rand_me_plz(rand_spec_one, rand_spec_two),
                             rand_me_plz(1, buf_size),
                             opt_other.at(rand_me_plz(0, opt_other.size() - 1)),
                             is_other, never_rand),
                strip_shell);
            if (oscar != "OOR") {
              sep_type = rand_me_plz(0, 1);
              if (sep_type == 0) {
                sys_str = " " + sys_str + *junk_opt + " " + oscar + " ";
              }
              if (sep_type == 1) {
                sys_str =
                    sys_str + " " + *junk_opt + other_sep + oscar + other_sep;
              }
            }
          }
        } else if (rand_buf == false) {
          for (std::vector<std::string>::const_iterator junk_opt_env =
                   junk_opts_env.begin();
               junk_opt_env != junk_opts_env.end();
               ++junk_opt_env) {  // loop through the vector of junk envs
            std::string oscar_env = remove_chars(
                make_garbage(rand_me_plz(rand_spec_one, rand_spec_two),
                             buf_size,
                             opt_other.at(rand_me_plz(0, opt_other.size() - 1)),
                             is_other, never_rand),
                " ");
            if (oscar_env != "OOR") {
              env_str = env_str + *junk_opt_env + oscar_env + " ";
            }
          }
          for (std::vector<std::string>::const_iterator junk_opt =
                   junk_opts.begin();
               junk_opt != junk_opts.end();
               ++junk_opt) {  // loop through the vector of junk opts
            std::string oscar = remove_chars(
                make_garbage(rand_me_plz(rand_spec_one, rand_spec_two),
                             buf_size,
                             opt_other.at(rand_me_plz(0, opt_other.size() - 1)),
                             is_other, never_rand),
                strip_shell);
            if (oscar != "OOR") {
              sep_type = rand_me_plz(0, 1);
              if (sep_type == 0) {
                sys_str = " " + sys_str + *junk_opt + " " + oscar + " ";
              }
              if (sep_type == 1) {
                sys_str = " " + sys_str + " " + *junk_opt + other_sep + oscar +
                          other_sep;
              }
            }
          }
        }
      }
      if (is_other == false) {
        if (rand_buf == true) {
          for (std::vector<std::string>::const_iterator junk_opt_env =
                   junk_opts_env.begin();
               junk_opt_env != junk_opts_env.end();
               ++junk_opt_env) {  // loop through the vector of junk envs
            std::string oscar_env = remove_chars(
                make_garbage(rand_me_plz(rand_spec_one, rand_spec_two),
                             rand_me_plz(1, buf_size), "", is_other,
                             never_rand),
                " ");
            if (oscar_env != "OOR") {
              env_str = env_str + *junk_opt_env + oscar_env + " ";
            }
          }
          for (std::vector<std::string>::const_iterator junk_opt =
                   junk_opts.begin();
               junk_opt != junk_opts.end();
               ++junk_opt) {  // loop through the vector of junk opts
            std::string oscar = remove_chars(
                make_garbage(rand_me_plz(rand_spec_one, rand_spec_two),
                             rand_me_plz(1, buf_size), "", is_other,
                             never_rand),
                strip_shell);
            if (oscar != "OOR") {
              sep_type = rand_me_plz(0, 1);
              if (sep_type == 0) {
                sys_str = " " + sys_str + *junk_opt + " " + oscar + " ";
              }
              if (sep_type == 1) {
                sys_str =
                    sys_str + " " + *junk_opt + other_sep + oscar + other_sep;
              }
            }
          }
        } else if (rand_buf == false) {
          for (std::vector<std::string>::const_iterator junk_opt_env =
                   junk_opts_env.begin();
               junk_opt_env != junk_opts_env.end();
               ++junk_opt_env) {  // loop through the vector of junk envs
            std::string oscar_env = remove_chars(
                make_garbage(rand_me_plz(rand_spec_one, rand_spec_two),
                             buf_size, "", is_other, never_rand),
                " ");
            if (oscar_env != "OOR") {
              env_str = env_str + *junk_opt_env + oscar_env + " ";
            }
          }
          for (std::vector<std::string>::const_iterator junk_opt =
                   junk_opts.begin();
               junk_opt != junk_opts.end();
               ++junk_opt) {  // loop through the vector of junk opts
            std::string oscar = remove_chars(
                make_garbage(rand_me_plz(rand_spec_one, rand_spec_two),
                             buf_size, "", is_other, never_rand),
                strip_shell);
            if (oscar != "OOR") {
              sep_type = rand_me_plz(0, 1);
              if (sep_type == 0) {
                sys_str = " " + sys_str + *junk_opt + " " + oscar + " ";
              }
              if (sep_type == 1) {
                sys_str =
                    sys_str + " " + *junk_opt + other_sep + oscar + other_sep;
              }
            }
          }
        }
      }
      std::vector<std::string> out_all =
          get_out_str(env_str, valgrind_str, sys_str, path_str, always_arg_after);
      std::string out_str = out_all[0];
      std::string out_str_p = out_all[1];
      junk_opts.clear();
      junk_opts.shrink_to_fit();
      junk_opts_env.clear();
      junk_opts_env.shrink_to_fit();
      if (debug == true) {
        std::ofstream w_f;
        w_f.open(write_file_n, std::ios::out | std::ios::app);
        w_f << out_str << std::endl << out_str_p << std::endl << std::endl;
        w_f.close();
        std::cout << out_str << std::endl << out_str_p << std::endl
                  << std::endl;
      }
      int pid;  // initializes child
      FILE *fp =
          popen2(out_str, "r", pid, low_lvl_user);  // opens child process fork
      char command_out[4096] = {0};
      std::stringstream output;
      while (read(fileno(fp), command_out, sizeof(command_out) - 1) != 0) {
        output << std::string(command_out);
        memset(&command_out, 0, sizeof(command_out));
      }
      pclose2(fp, pid);
      if (run_command != "") {
        int run_com_pid;  // initializes child
        FILE *fp = popen2(run_command, "r", run_com_pid,
                          low_lvl_user);  // opens child process fork
        pclose2(fp, run_com_pid);
      }
      std::thread reaper_thread(
          reaper, pid,
          t_timeout);  // takes care of killing it off if it takes too long
      reaper_thread.detach();
      std::string token;
      while (std::getline(output, token)) {
        std::smatch sf;
        if (regex_search(token, sf, sf_reg)) {  // match crash
          std::cout << token << std::endl;
          std::cout << "Crashed with command: " << std::endl << out_str_p
                    << std::endl;
          if (junk_file_of_args != "") {
            std::cout << "File data left in: " << junk_file_of_args
                      << std::endl;
          }
          if (write_to_file == true) {
            write_seg(write_file_n, out_str_p);
            std::cout << "Crash logged." << std::endl;
            exit(0);
          } else {
            exit(0);
          }
        }
      }
    }
  }

  else {
    std::cerr << "Command not found at path..." << std::endl;
    exit(1);
  }
}
