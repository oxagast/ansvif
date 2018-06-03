/*
 * ansvif
 * A Not So Very Intelligent Fuzzer
 * Marshall Whittaker / oxagast
 */

//    __ _  _  __   ___  __  ____ ____
//   /  ( \/ )/ _\ / __)/ _\/ ___(_  _)
//  (  O )  (/    ( (_ /    \___ \ )(
//   \__(_/\_\_/\_/\___\_/\_(____/(__)

#ifdef __unix
#include "src/version.h"
#endif
#ifdef _WIN32
#include "version.h"
#endif
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <regex>
#include <signal.h>
#include <stdio.h>
#include <sys/stat.h>
#include <thread>
#include <unistd.h>
#include <vector>

#define READ 0
#define WRITE 1

std::string make_garbage(int trash, int buf, std::string opt_other_str,
                         bool is_other, bool never_rand);
std::string trash_generator(int trash, int buf, std::string user_junk,
                            std::string opt_other_str, bool never_rand);
int rand_me_plz(int rand_from, int rand_to);
void help_me(std::string mr_me);

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
  /* Cashhhhhhhhh me ousside howbow dat?? */
  /* if there is an old file we should remove it first */
  remove(filename.c_str());
  /* initialize our junk and write oscar to the file */
  std::string oscar;
  std::ofstream w_f;
  w_f.open(filename, std::ios::out | std::ios::app);
  for (int start_buf = 0; start_buf <= buf_size; start_buf++) {
    std::string oscar = opt_other.at(rand_me_plz(0, opt_other.size() - 1));
    std::string trash =
        make_garbage(rand_me_plz(rand_spec_one, rand_spec_two),
                     rand_me_plz(1, buf_size), "", false, never_rand);
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
