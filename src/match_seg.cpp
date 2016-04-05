/*
 * ansvif
 * A Not So Very Intelligent Fuzzer
 * Marshall Whittaker / oxagast
 */

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
std::vector<std::string> get_out_str(std::string env_str,
                                     std::string valgrind_str,
                                     std::string sys_str, std::string path_str,
                                     std::string always_arg_after, std::string fuzz_after);
std::vector<std::string> get_out_str_pc(std::string env_str,
                                     std::string valgrind_str,
                                     std::string sys_str, std::string path_str,
                                     std::string always_arg_after, std::string fuzz_after);
bool match_seg(int buf_size, std::vector<std::string> opts,
               std::vector<std::string> spec_env, std::string path_str,
               std::string strip_shell, bool rand_all, bool write_to_file,
               std::string write_file_n, bool rand_buf,
               std::vector<std::string> opt_other, bool is_other,
               std::string other_sep, int t_timeout, std::string low_lvl_user,
               std::string junk_file_of_args, std::string always_arg_before,
               std::string always_arg_after, bool never_rand,
               std::string run_command, std::string fault_code, bool valgrind,
               bool single_try, bool percent_sign, bool verbose, bool debug) {
  bool segged = false;
  std::string valgrind_str;
  if (valgrind == true) {
    valgrind_str =
        "/usr/bin/valgrind --leak-check=full --xml=yes "
        "--xml-file=ansvif.valgrind.log --error-exitcode=139";  // if valgrind
                                                                // is activated
                                                                // we use exit
                                                                // code 139 to
                                                                // wrap around
                                                                // and match
                                                                // faults
  }
  if (valgrind == false) {
    valgrind_str = "";  // nada if valgrinds out
  }
  if (file_exists(path_str) == true) {  // check to make sure that the command
                                        // we are looking for actually is a
                                        // executable file
    while (segged == false) {
      int rand_spec_one, rand_spec_two;  // initialize the two random 'pieces'
      if (rand_all == true) {
        rand_spec_one = 3;  // make for always random data
        rand_spec_two = 3;  // always random too
      } else {
        rand_spec_one = 0;   // any data entered
        rand_spec_two = 12;  // any data entered
      }
      std::vector<std::string>
          junk_opts_env;  // options for the environment in a vector
      std::vector<std::string> junk_opts;  // other options
      std::string env_str;                 // initialize the environment string
      std::string sys_str = always_arg_before + " ";  // the string to execute,
                                                      // with the argument thats
                                                      // always before it first
      if (junk_file_of_args != "") {
        write_junk_file(junk_file_of_args, opt_other, buf_size, rand_spec_one,
                        rand_spec_two, never_rand, other_sep,
                        verbose);  // write the file full of arguments
      }
      int sep_type;                 // the type of separator, as an integer
      int opts_size = opts.size();  // so that we don't have a warning about
                                    // unsigned with -Wall
      int my_penis_size = spec_env.size();  // also so we have a don't have
                                            // warning about my penis being very
                                            // large
      for (int cmd_flag_l = 0; cmd_flag_l < opts_size;
           cmd_flag_l++) {             // loop around the options
        if (rand_me_plz(0, 1) == 1) {  // roll tha die
          junk_opts.push_back(
              opts.at(cmd_flag_l));  // put the random arg in the vector
        }
      }
      for (int cmd_flag_a = 0; cmd_flag_a < my_penis_size;
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
            if (oscar_env != "OOR") {  // if not out of range then do that
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
            if (oscar != "OOR") {  // ditto
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
            if (oscar_env != "OOR") {  // this is getting really repetative
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
            if (oscar != "OOR") {  // if not out of range
              sep_type = rand_me_plz(
                  0, 1);  // randomize weather we have a space or not...
              if (sep_type == 0) {  // if we don't have a space...
                sys_str = " " + sys_str + *junk_opt + " " + oscar + " ";
              }
              if (sep_type == 1) {  // if we have a space...
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
      std::string fuzz_after = remove_chars(make_garbage(rand_me_plz(rand_spec_one, rand_spec_two), buf_size, "", is_other, never_rand), strip_shell);
      std::vector<std::string> out_all;
      if (percent_sign == true) {
        out_all = get_out_str_pc(
          env_str, valgrind_str, sys_str, path_str,
          always_arg_after, fuzz_after);  // all the stuff put together to send out (aside
                              // from echo $? which is done in sys_string
      }
      if (percent_sign == false) {
        out_all = get_out_str(
          env_str, valgrind_str, sys_str, path_str,
          always_arg_after, fuzz_after);  // all the stuff put together to send out (aside
                              // from echo $? which is done in sys_string
      }
      std::string out_str = out_all[0];    // first one in vector is the string
                                           // without the printf stuff
      std::string out_str_p = out_all[1];  // second one in vector has the
                                           // printf stuff (for reproduction)
      junk_opts.clear();                   // clear the vector
      junk_opts.shrink_to_fit();           // shrink it to save mem, hope we mem
                                           // manage... mirite?
      junk_opts_env.clear();               // clear env vector
      junk_opts_env.shrink_to_fit();       // shrink env vector
      if (debug == true) {                 // if we are debugging...
        std::ofstream w_f;
        w_f.open(write_file_n, std::ios::out | std::ios::app);  // open...
        w_f << out_str << std::endl << out_str_p << std::endl
            << std::endl;  // write...
        w_f.close();  // then close if we have a file then write all the junk
                      // out to that too
        std::cout << out_str << std::endl << out_str_p << std::endl
                  << std::endl;  // write ALL the junk out to stdout
      }

      int pid;  // initializes child
      FILE *fp =
          popen2(out_str, "r", pid, low_lvl_user);  // opens child process fork
      char command_out[4096] = {0};  // 4096 bytes should be enough to handle
                                     // whatever pops out where we match
      std::stringstream output;      // we use stringstream to get the output
      while (read(fileno(fp), command_out, sizeof(command_out) - 1) !=
             0) {  // while the commands being run read it
        output << std::string(
            command_out);  // out its content into output stringstream
        memset(&command_out, 0,
               sizeof(command_out));  // make sure we don't overflow
      }
      pclose2(fp, pid);  // close out the command cleanly
      int run_com_pid;  // initializes child
      if (run_command != "") {
        FILE *fp = popen2(out_str, "r", run_com_pid,
                          low_lvl_user);  // opens child process fork
        pclose2(fp, run_com_pid);         // close out the command cleanly
      }
      std::thread reaper_thread(reaper, pid,
                                t_timeout);  // takes care of killing it off if
                                             // it takes too long... this is
                                             // unclean
      reaper_thread.detach();                // takes care of the reaper thread
      std::string cmd_output;  // the initializse thing that will have our cmd
                               // output string in it that comes out...
      while (std::getline(output, cmd_output)) {
       if ((cmd_output == "132") || (cmd_output == "134") ||
            (cmd_output == "139") || (cmd_output == "135") ||
            (cmd_output == "136") || (cmd_output == "159") ||
            (cmd_output ==
             fault_code)) {  // default fault codes, plus the fault
                             // code the user specified (or dummy
                             // code)
          std::cout << "pid: " << pid << "\n";
          std::cout << "Crashed with command: " << std::endl << out_str_p
                    << std::endl;  // write out that we crashed with command x
          if (junk_file_of_args != "") {
            std::cout << "File data left in: " << junk_file_of_args
                      << std::endl;  // ...make sure to log the file data too
          }
          if (write_to_file == true) {  // if we are writing to a file...
            write_seg(write_file_n, "pid: " + std::to_string(pid) + "\n" + "Crashed with command: ");  // write the pid if we're logging
            write_seg(write_file_n, out_str_p);  // call the subroutine to write
                                                 // the fault to a log (comes
                                                 // from common)
            std::cout << "Crash logged."
                      << std::endl;  //  tells us that we crashed.
            exit(0);  // exit with a clean/dirty code (haha, get it? clean
                      // because it crashed?)
          } else {
            exit(0);  // same as above
          }
        }
      }
      if (single_try == true) {  // do all that shit but ONLY once
        if ((verbose == true) || (debug == true)) {
          std::cout << "No fault of mine!" << std::endl;  // if we're talking
                                                          // alot then tell us
                                                          // that we're done.
        }
        exit(64);  // send a non standard error code to tell a wrapper that we
                   // didn't hit a fault
      }
    }
    exit(0);  // exit cleanly
  }

  else {
    std::cerr << "Command not found at path..." << std::endl;  // er wut???
    exit(1);                                                   // sorry guize
  }
}
