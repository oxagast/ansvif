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
#include <thread>
#include <unistd.h>
#include <vector>
#include <signal.h>
#include <random>
#include "src/main.h"

#define READ 0
#define WRITE 1


std::string make_garbage(int trash, int buf, std::string opt_other_str,
                         bool is_other, bool never_rand);
std::string trash_generator(int trash, int buf, std::string user_junk,
                            std::string opt_other_str, bool never_rand);
bool match_seg(int buf_size, std::vector<std::string> opts,
               std::vector<std::string> spec_env, std::string path_str,
               std::string strip_shell, bool rand_all, bool write_to_file,
               std::string write_file_n, bool rand_buf,
               std::vector<std::string> opt_other, bool is_other,
               std::string other_sep, int t_timeout, std::string low_lvl_user,
               std::string junk_file_of_args, std::string always_arg_before,
               std::string always_arg_after, bool never_rand,
               std::string run_command, std::string fault_code, bool single_try,
               bool percent_sign, int static_args, bool, bool keep_going, bool verbose, bool debug);
void help_me(std::string mr_me);
std::vector<std::string> get_flags_man(std::string man_page,
                                       std::string man_loc, bool verbose,
                                       bool debug, bool dump_opts);
int rand_me_plz(int rand_from, int rand_to);

std::string remove_chars(const std::string &source, const std::string &chars) {
  /* initialize an empty removal string */
  std::string result = "";
  for (unsigned int i = 0; i < source.length(); i++) {
    /* loop through each character removing the ones we
     * don't want
     */
    bool foundany = false;
    for (unsigned int j = 0; j < chars.length() && !foundany; j++) {
      foundany = (source[i] == chars[j]);
    }
    if (!foundany) {
      /* put it back in the buffer if it's not in the list */
      result += source[i];
    }
  }
  return(result);
}

int toint(std::string ints, std::string my_prog) {
     std::istringstream b_size(ints);
  int is_int_b_s;
  if (!(b_size >> is_int_b_s)) {
    help_me(my_prog);
  }
  char buf_char_maybe_b_s;
  if (b_size >> buf_char_maybe_b_s) {
    help_me(my_prog);
  } else {
  /* for compatibility with cygwin */
  return atoi(ints.c_str());
  }
  return(0);
}

bool file_exists(const std::string &filen) {
  struct stat buf;
  /* return if the file exists or not */
  return (stat(filen.c_str(), &buf) == 0);
}

int reaper(int grim, int t_timeout) {
#ifdef __linux
  /* run the timer and after the timeout we'll run
   * SIGKILL on it (kill -9 equivilant on linux)
   */
  sleep(t_timeout);
  kill(grim, 9);
  return (0);
  #elif _WIN32
  /* windows doesn't support kill 9 */
#else

#endif
}

std::vector<std::string> get_flags_template(std::string filename, bool verbose,
                                            bool debug) {
  /* initialize an options vector and push each option
   * back onto the vector from the template file
   */
  std::vector<std::string> opt_vec;
  std::string line;
  std::ifstream template_file(filename);
  if (template_file.is_open()) {
    while (std::getline(template_file, line)) {
      opt_vec.push_back(line);
    }
    /* we should close it */
    template_file.close();
  } else {
    /* this is incase they supplied a file that wasn't
     * available for some reason
     */
    std::cerr << "Could not open template file..."
              << std::endl;
    exit(1);
  }
  return (opt_vec); // return the vector with the options
}

std::vector<std::string> get_other(std::string filename,
                                   bool verbose, bool debug) {
  /* this is all pretty much the same as above */
  std::vector<std::string> other_vec;
  std::string line;
  std::ifstream other_file(filename);
  if (other_file.is_open()) {
    while (std::getline(other_file, line)) {
      other_vec.push_back(line);
    }
    other_file.close();
  } else {
    std::cerr << "Could not open template file..." << std::endl;
    exit(1);
  }
  return (other_vec);
}

void write_seg(std::string filename, std::string line) {
  /* this is just a simple file writing routine
   * used mostly for logging and writing the junk
   * files
   */
  std::ofstream w_f;
  w_f.open(filename, std::ios::out | std::ios::app);
  w_f << line << std::endl;
  w_f.close();
}

void write_junk_file(std::string filename, std::vector<std::string> opt_other,
                     int buf_size, int rand_spec_one, int rand_spec_two,
                     bool never_rand, std::string other_sep, bool verbose) {
  /* if there is an old file we should remove it first */
  remove(filename.c_str());
  /* initialize our junk and write oscar to the file */
  std::string oscar;
  std::ofstream w_f;
  w_f.open(filename, std::ios::out | std::ios::app);
  for (int start_buf = 0; start_buf <= buf_size; start_buf++) {
    std::string oscar = opt_other.at(
        rand_me_plz(0, opt_other.size() - 1));
    std::string trash = make_garbage(rand_me_plz(rand_spec_one, rand_spec_two),
                                     rand_me_plz(1, buf_size), "", false,
                                     never_rand);
    w_f << oscar;
    if (trash != "OOR") {
      w_f << trash;
    }
    int is_sep = rand_me_plz(0, 1);
    if (is_sep == 1) {
      w_f << other_sep << std::endl;
    } else {
      w_f << std::endl;
    }
    if (verbose == true) {
      std::cerr << oscar;
      {
        /* write the shit to terminal if we're in verbose mode */
        if (trash != "OOR")
          std::cerr << trash;
      }
      if (is_sep == 1) {
        std::cerr << other_sep << std::endl;
      } else {
        std::cerr << std::endl;
      }
    }
  }
  /* make sure to close it out */
  w_f.close();
}
