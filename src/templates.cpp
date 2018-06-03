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
#include <random>
#include <regex>
#include <stdio.h>
#include <unistd.h>
#include <vector>

std::vector<std::string> get_flags_man(std::string man_page,
                                       std::string man_loc, bool verbose,
                                       bool debug, bool dump_opts);
int rand_me_plz(int rand_from, int rand_to);
void help_me(std::string mr_me);

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
    std::cerr << "Could not open template file..." << std::endl;
    help_me("ansvif");
  }
  return (opt_vec); // return the vector with the options
}

std::vector<std::string> get_other(std::string filename, bool verbose,
                                   bool debug) {
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
    help_me("ansvif");
  }
  return (other_vec);
}
