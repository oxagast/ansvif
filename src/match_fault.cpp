/*
 * ansvif
 * A Not So Very Intelligent Fuzzer
 * Marshall Whittaker / oxagast
 */

//    __ _  _  __   ___  __  ____ ____
//   /  ( \/ )/ _\ / __)/ _\/ ___(_  _)
//  (  O )  (/    ( (_ /    \___ \ )(
//   \__(_/\_\_/\_/\___\_/\_(____/(__)

#ifdef __NOTANDROID
#include "../include/xmlwriter/xml_writer.hpp"
#endif
#ifdef __ANDROID__
#include "../include/xmlwriter/xml_writer.hpp"
#endif
#ifdef _WIN32
#include "version.h"
#endif
#include "../include/md5.h"
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <regex>
#include <sstream>
#include <stdio.h>
#include <string>
#include <thread>
#include <unistd.h>
#include <vector>

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
} o;

struct RunCommands {
  public:
  std::string run_command;
  std::string before_command;
  std::string prog_name;
}runit;


struct Out {
  public:
  std::string o;
  std::string p;
}out;
       
struct Monopoly {
  public:
  bool keep_going;
  bool single_try;
} go;
      
struct BuffCont {
  public:
  int buf_size_int;
  bool rand_buf;
} bufctl;
void log_hang(std::string write_file_n, std::string out_str_p,
              std::string out_str, std::string junk_file_of_args, int pid);
void log_tail(std::string write_file_n, std::string junk_file_of_args,
              std::string output_logfile, std::string crash_logfile,
              std::string cmd_output, std::string out_str_p,
              std::string out_str, int pid);
void log_head(std::string write_file_n, std::string path_str,
              std::string cmd_output, std::string out_str_p,
              std::string out_str, int pid);
std::string remove_chars(const std::string &source, const std::string &chars);
int reaper(int grim, int t_timeout, std::string prog_name);
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
std::string
get_out_str(std::string env_str, std::string valgrind_str, std::string sys_str,
            std::string path_str, std::string always_arg_before,
            std::string always_arg_after, std::string fuzz_after,
            std::string log_prefix, std::string before_command);
std::string
get_out_str_pc(std::string env_str, std::string valgrind_str,
               std::string sys_str, std::string path_str,
               std::string always_arg_before, std::string always_arg_after,
               std::string fuzz_after, std::string log_prefix,
               std::string before_command);
bool match_seg(struct Options o, struct RunCommands runit, struct Monopoly go, struct BuffCont bufctl) {
  bool segged = false;
  std::vector<std::string> used_token;
  bool is_other = false;
  if (o.opt_other.size() != 0) {
	  is_other = true;
  }
  std::string valgrind_str;
  std::string output_logfile = o.write_file_n + ".output.ansvif.log";
  std::string crash_logfile = o.write_file_n + ".crash.ansvif.log";
  std::string valgrind_logfile = o.write_file_n + ".valgrind.ansvif.log";
  if (o.valgrind == true) {
    if (o.write_file_n != "") {
      /* if valgrind is active then we'll tel lit to use
       * an error code (139) as a fault code so that we
       * can trap it, it will also be logging if we are
       */
      valgrind_str = "/usr/bin/valgrind --leak-check=full --xml=yes "
                     "--xml-file=" +
                     o.write_file_n +
                     ".valgrind.ansvif.log --error-exitcode=139";
    } else {
      /* also for the valgrind wrapper but this time we're
       * not logging
       */
      valgrind_str = "/usr/bin/valgrind --leak-check=full --error-exitcode=139";
    }
  }
  if (o.valgrind == false) {
    /* not using valgrind */
    valgrind_str = "";
  }
  if (file_exists(o.path_str) == true) {
    /* check to make sure the file we're going to run
     * is actually an executable file
     */
    while (segged == false) {
      unlink(crash_logfile.c_str());
      unlink(output_logfile.c_str());
      unlink(valgrind_logfile.c_str());
      /* initialize two random pieces */
      int rand_spec_one, rand_spec_two;
      std::vector<std::string> junk_opters;

      if (o.rand_all == true) {
        /* or always random data (8) */
        rand_spec_one = 8;
        rand_spec_two = 8;
      } else {
        /* any data entered */
        rand_spec_one = 0x00;
        rand_spec_two = 0x21;
      }
      /* initilize our optiosn vectors, our environment string
       * and our system call string
       */
      std::vector<std::string> junk_opts_env, junk_opts;
      std::string env_str, sys_str;
      if (o.junk_file_of_args != "") {
        /* write a junk file */
        write_junk_file(o.junk_file_of_args, o.opt_other, bufctl.buf_size_int,
                        rand_spec_one, rand_spec_two, o.never_rand, o.other_sep,
                        o.verbose);
      }
      int sep_type;
      /* so we don't have warnings with -Wall */
      int opts_size = o.opts.size();
      int my_penis_size = o.spec_env.size();
      /* loop around the options, roll the die, and put a
       * random argument in the vector
       */
      if (o.static_args == 0) {
        for (int cmd_flag_l = 0; cmd_flag_l < opts_size; cmd_flag_l++) {
          if (rand_me_plz(0, 1) == 1) {
            junk_opts.push_back("'" + o.opts.at(cmd_flag_l) + "'");
          }
        }
      }
      if (o.static_args != 0) {
        for (int cmd_flag_l = 0; cmd_flag_l < opts_size; cmd_flag_l++) {
          if ((rand_me_plz(0, 1) == 1) && (cmd_flag_l < opts_size)) {
            junk_opts.push_back("'" + o.opts.at(cmd_flag_l) + "'");
          } else {
            junk_opts.push_back(" ");
          }
        }
        std::random_shuffle(junk_opts.begin(), junk_opts.end());
        for (int holder = 0; holder < o.static_args; holder++) {
          junk_opters.push_back(
              "'" + o.opts.at(rand_me_plz(0, o.opts.size() - 1)) + "'");
        }
      }
      junk_opts = junk_opters;
      for (int cmd_flag_a = 0; cmd_flag_a < my_penis_size; cmd_flag_a++) {
        if (rand_me_plz(0, 1) == 1) {
          junk_opts_env.push_back(o.spec_env.at(cmd_flag_a));
        }
      }
      if (is_other == true) {
        if (bufctl.rand_buf == true) {
          for (std::vector<std::string>::const_iterator junk_opt_env =
                   junk_opts_env.begin();
               junk_opt_env != junk_opts_env.end(); ++junk_opt_env) {
            /* environment variable random shit */
            std::string oscar_env = remove_chars(
                make_garbage(
                    rand_me_plz(rand_spec_one, rand_spec_two),
                    rand_me_plz(1, bufctl.buf_size_int),
                    o.opt_other.at(rand_me_plz(0, o.opt_other.size() - 1)),
                    is_other, o.never_rand),
                " ");
            if (oscar_env != "'OOR'") {
              /* making sure it's not out of range */
              env_str = env_str + *junk_opt_env + oscar_env;
            }
          }

          for (std::vector<std::string>::const_iterator junk_opt =
                   junk_opts.begin();
               junk_opt != junk_opts.end(); ++junk_opt) {
            std::string oscar = remove_chars(
                make_garbage(
                    rand_me_plz(rand_spec_one, rand_spec_two),
                    rand_me_plz(1, bufctl.buf_size_int),
                    o.opt_other.at(rand_me_plz(0, o.opt_other.size() - 1)),
                    is_other, o.never_rand),
                o.strip_shell);
            if (oscar != "'OOR'") {
              sep_type = rand_me_plz(0, 1);
              if (sep_type == 0) {
                sys_str = sys_str + *junk_opt + " " + oscar;
              }
              if (sep_type == 1) {
                sys_str =
                    sys_str + *junk_opt + o.other_sep + oscar + o.other_sep;
              }
            }
          }
        } else if (bufctl.rand_buf == false) {
          for (std::vector<std::string>::const_iterator junk_opt_env =
                   junk_opts_env.begin();
               junk_opt_env != junk_opts_env.end(); ++junk_opt_env) {
            std::string oscar_env = remove_chars(
                make_garbage(
                    rand_me_plz(rand_spec_one, rand_spec_two), bufctl.buf_size_int,
                    o.opt_other.at(rand_me_plz(0, o.opt_other.size() - 1)),
                    is_other, o.never_rand),
                " ");
            if (oscar_env != "'OOR'") {
              /* really really repetative */
              env_str = env_str + *junk_opt_env + " " + oscar_env;
            }
          }
          for (std::vector<std::string>::const_iterator junk_opt =
                   junk_opts.begin();
               junk_opt != junk_opts.end(); ++junk_opt) {
            std::string oscar = remove_chars(
                make_garbage(
                    rand_me_plz(rand_spec_one, rand_spec_two), bufctl.buf_size_int,
                    o.opt_other.at(rand_me_plz(0, o.opt_other.size() - 1)),
                    is_other, o.never_rand),
                o.strip_shell);
            if (oscar != "'OOR'") { // if not out of range
              /* here we randomize if we have a space or not */
              sep_type = rand_me_plz(0, 1);
              if (sep_type == 0) {
                /* space */
                sys_str = sys_str + " " + *junk_opt + " " + oscar;
              }
              if (sep_type == 1) {
                /* some other seperator like a comma or w/e */
                sys_str = sys_str + " " + *junk_opt + o.other_sep + oscar +
                          o.other_sep;
              }
            }
          }
        }
      }
      if (is_other == false) {
        if (bufctl.rand_buf == true) {
          for (std::vector<std::string>::const_iterator junk_opt_env =
                   junk_opts_env.begin();
               junk_opt_env != junk_opts_env.end(); ++junk_opt_env) {
            /* loop through the vector of junk environment
             * variables
             */
            std::string oscar_env = remove_chars(
                make_garbage(rand_me_plz(rand_spec_one, rand_spec_two),
                             rand_me_plz(1, bufctl.buf_size_int), "", is_other,
                             o.never_rand),
                " ");
            if (oscar_env != "'OOR'") {
              env_str = env_str + *junk_opt_env + oscar_env + " ";
            }
          }
          for (std::vector<std::string>::const_iterator junk_opt =
                   junk_opts.begin();
               junk_opt != junk_opts.end(); ++junk_opt) {
            /* loop through the vector of junk options */
            std::string oscar = remove_chars(
                make_garbage(rand_me_plz(rand_spec_one, rand_spec_two),
                             rand_me_plz(1, bufctl.buf_size_int), "", is_other,
                             o.never_rand),
                o.strip_shell);
            if (oscar != "'OOR'") {
              sep_type = rand_me_plz(0, 1);
              if (sep_type == 0) {
                sys_str = sys_str + " " + *junk_opt + " " + oscar;
              }
              if (sep_type == 1) {
                sys_str =
                    sys_str + *junk_opt + o.other_sep + oscar + o.other_sep;
              }
            }
          }
        } else if (bufctl.rand_buf == false) {
          for (std::vector<std::string>::const_iterator junk_opt_env =
                   junk_opts_env.begin();
               junk_opt_env != junk_opts_env.end(); ++junk_opt_env) {
            std::string oscar_env = remove_chars(
                make_garbage(rand_me_plz(rand_spec_one, rand_spec_two),
                             bufctl.buf_size_int, "", is_other, o.never_rand),
                " ");
            if (oscar_env != "'OOR'") {
              env_str = env_str + *junk_opt_env + " " + oscar_env;
            }
          }

          for (std::vector<std::string>::const_iterator junk_opt =
                   junk_opts.begin();
               junk_opt != junk_opts.end(); ++junk_opt) {

            std::string oscar = remove_chars(
                make_garbage(rand_me_plz(rand_spec_one, rand_spec_two),
                             bufctl.buf_size_int, "", is_other, o.never_rand),
                o.strip_shell);
            if (oscar != "'OOR'") {
              sep_type = rand_me_plz(0, 1);
              if (sep_type == 0) {
                sys_str = sys_str + *junk_opt + " " + oscar;
              }
              if (sep_type == 1) {
                sys_str =
                    sys_str + *junk_opt + o.other_sep + oscar + o.other_sep;
              }
            }
          }
        }
      }
      std::string fuzz_after = remove_chars(
          make_garbage(rand_me_plz(rand_spec_one, rand_spec_two),
                       bufctl.buf_size_int, "", is_other, o.never_rand),
          o.strip_shell);
      Out out_str;
      if (fuzz_after == "'OOR'") {
        fuzz_after = "";
      }
      if (o.percent_sign == true) {
        /* all of the stuff except for echo $? is now
         * put together and ready to be sent out to the
         * subroutine, $? is done in sys_string.cpp
         */
        out_str.p = get_out_str_pc(env_str, valgrind_str, sys_str, o.path_str,
                                 o.always_arg_before, o.always_arg_after,
                                 fuzz_after, o.write_file_n, runit.before_command);
      }
      if (o.percent_sign == false) {
        out_str.o = get_out_str(env_str, valgrind_str, sys_str, o.path_str,
                              o.always_arg_before, o.always_arg_after,
                              fuzz_after, o.write_file_n, runit.before_command);
      }
      /* coming to the stuff from sys_string either
       * normal or printf output
       */
      junk_opts.clear();
      junk_opts.shrink_to_fit();
      junk_opts_env.clear();
      junk_opts_env.shrink_to_fit();
      MD5 md5;
      char *tried = new char[out_str.o.length() + 1];
      strcpy(tried, out_str.o.c_str());
      char *tried_p = tried;
      std::string h_output = md5.digestString(tried_p);
      if (std::find(used_token.begin(), used_token.end(), h_output) !=
          used_token.end()) {
        used_token.push_back(h_output);
      } else {
        used_token.push_back(h_output);
#ifdef __linux
        if (bufctl.buf_size_int == 0) {
          out_str.o = runit.before_command + " " + o.path_str + " " +
                    o.always_arg_before + " " + o.always_arg_after;
          out_str.p = runit.before_command + " " + o.path_str + " " +
                      o.always_arg_before + " " + o.always_arg_after;
          if (o.write_file_n == "") {
            /* incase we are logging don't leave a blank file */
            out_str.o = out_str.o + " >/dev/null 2>&1; echo $?";
          } else {
            /* get the signal here and log */

            out_str.o = out_str.o + " >" + o.write_file_n +
                      ".output.ansvif.log 2>&1; echo $?";
          }
        }
#endif
        if (o.debug == true) {
          /* write ALL the junk to STDOUT since we're in
           * debug mode
           */
          std::cout << out_str.o << std::endl
                    << out_str.p << std::endl
                    << std::endl;
        }
        /* this here takes care of the command that is run after
         * the fuzz
         */
        if (runit.run_command != "") {
#ifdef _WIN32
          runit.run_command = "powershell " + runit.run_command;
#endif
          int run_com_pid;
          FILE *fp = popen2(runit.run_command, "r", run_com_pid, o.low_lvl_user);
          pclose2(fp, run_com_pid);
        }
        /* inititalize the child and open the child process fork
         * 4096 bytes should be enough to handle whatever pops out
         * when we go to match.  we match with stringstream to get
         * output and put its contense in 'output'
         */
        int pid;
        FILE *fp = popen2(out_str.o, "r", pid, o.low_lvl_user);
        /* this takes care of killing off the child if it takes
         *  * too long
         *   */

        /*
        #ifdef __unix__
                std::thread reaper_thread(reaper, pid, o.t_timeout,
        o.prog_name); reaper_thread.detach(); #endif
        */
        char command_out[4096] = {0};
        std::stringstream output;
        while (read(fileno(fp), command_out, sizeof(command_out) - 1) != 0) {
          output << std::string(command_out);
          /* make sure we don't overflow */
          memset(&command_out, 0, sizeof(command_out));
        }
        /* close out the command cleanly */
        pclose2(fp, pid);
        /* our output will be stored here! */
        std::string cmd_output = output.str();
        if (o.write_file_n != "") {
          /* logging */
          log_head(o.write_file_n, o.path_str, cmd_output, out_str.p, out_str.o,
                   pid);
        }
#ifdef __ANDROID__
        if (file_exists("/sdcard/ansvif/crashed") == true) {
#endif
#ifdef __NOTANDROID__
          if (file_exists("/tmp/a.crashed") == true) {
#endif
#ifdef __WIN32
            std::size_t foundW1073741819 = cmd_output.find("-1073741819");
            std::size_t foundW1073740791 = cmd_output.find("-1073740791");
            std::size_t foundW1073741571 = cmd_output.find("-1073741571");
            std::size_t foundW532459699 = cmd_output.find("-532459699");
            /*
             * Windows codes:
             * -1073741819 -1073740791 -1073741571 -532459699
             * Unix codes:
             * 132 134 139 135 136 159 138
             * also some people call me the space cowboy
             */
            std::size_t foundother =
                cmd_output.find("CRASHCODE " + o.fault_code);
        if ((foundW1073741819 != std::string::npos) ||
	(foundW1073740791 != std::string::npos) ||
	(foundW1073741571 != std::string::npos) ||
	(foundW532459699 != std::string::npos)) {
              cmd_output = cmd_output.replace(0, 22, "");
#endif
#ifdef __unix__
              std::cout << "PID: " << pid << std::endl;
#endif
              std::cout << "Exit Code: " << cmd_output << std::endl;
              std::cout << "Crashed with command: " << std::endl
                        << out_str.o << std::endl;
              if (o.junk_file_of_args != "") {
                /* log the file data too */
                std::cout << "File data left in: " << o.junk_file_of_args
                          << std::endl;
              }
              if (o.write_file_n != "") {
                /* since we crashed we're going to finish writing to the
                 * xml file
                 */
                log_tail(o.write_file_n, o.junk_file_of_args, output_logfile,
                         crash_logfile, cmd_output, out_str.p, out_str.o, pid);
                /* then exit cleanly because we crashed it! Get it? :) */
                /* logging hangs */
              }
              if (go.keep_going == false) {
                return (false);
              }
        }  // For the checksum algorithm under linux
          }
          if (go.single_try == true) {
            /* do all that shit but only once! */
            if ((o.verbose == true) || (o.debug == true)) {
              std::cout << "No fault of mine!" << std::endl;
            }
            /* this non standard code 64 is to tell a wrapper that
             * we never hit a fault with the single run
             */
            exit(64);
          }
        }
      }
      return (true);
    }
