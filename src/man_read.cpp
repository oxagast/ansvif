/*
 * ansvif
 * A Not So Very Intelligent Fuzzer
 * Marshall Whittaker / oxagast
 */

//    __ _  _  __   ___  __  ____ ____ 
//   /  ( \/ )/ _\ / __)/ _\/ ___(_  _)
//  (  O )  (/    ( (_ /    \___ \ )(  
//   \__(_/\_\_/\_/\___\_/\_(____/(__)
#ifdef __linux
#include "../include/gzstream/gzstream.h"
#endif
#include <cstring>
#include <regex>
#include <thread>
#include <unistd.h>
#include <vector>

std::string remove_chars(const std::string &source, const std::string &chars);
bool file_exists(const std::string &filen);

std::vector<std::string> get_flags_man(std::string man_page,
                                       std::string man_loc, bool verbose,
                                       bool debug, bool dump_opts) {
#ifdef __linux
  /* declare our variables */
  std::string filename;
  std::vector<std::string> opt_vec;
  /* put the filename back together */
  filename = "/usr/share/man/man" + man_loc + "/" + man_page + "." + man_loc +
             ".gz";
  if (file_exists(filename) == true) {
    /* change our type for chr_fn */
    char *chr_fn = strdup(filename.c_str());
    /* time for the gunzip! */
    igzstream in(chr_fn);
    std::string gzline;
    /* now we have some hella fucking regexes that are
     * going to be matched... fml
     */
    std::regex start_of_opt_1(
        "^(\\.?\\\\?\\w{2} )*(\\\\?\\w{2} ?)*(:?\\.B "
        ")*((?:(?:\\\\-)+\\w+)(?:\\\\-\\w+)*).*");
    std::smatch opt_part_1;
    std::regex start_of_opt_2(
        "^\\.Op Fl (\\w+) Ar.*");
    std::smatch opt_part_2;
    std::regex start_of_opt_3("^\\\\fB(-.*)\\\\fB.*");
    std::smatch opt_part_3;
    while (std::getline(in, gzline)) {
      /* if we've got a manpage, then we match the stuff
       * out of the regex as we're ungzing and putting the 
       * options in strings, removing the backspaces, and 
       * putting it all cleanly into a vector
       */
      if (std::regex_match(gzline, opt_part_1,
                           start_of_opt_1)) {
        std::string opt_1 = opt_part_1[4];
        std::string opt_release = (remove_chars(
            opt_part_1[4], "\\"));
        opt_vec.push_back(opt_release);
      }
      if (std::regex_match(
              gzline, opt_part_2,
              start_of_opt_2)) {
        std::string opt_2 = opt_part_2[1];
        opt_vec.push_back("-" + opt_2);
      }
      if (std::regex_match(
              gzline, opt_part_3,
              start_of_opt_3)) {
        std::string opt_3 = opt_part_3[1];
        opt_vec.push_back(opt_3);
      }
    }
  } else {
    /* either they didn't have the right location or the command
     * doesn't have a manpage, either way, exit with error 1
     */
    std::cerr << "Could not find a manpage for that command..."
              << std::endl;
    exit(1);
  }
  /* sort them so that we can unique them correctly */
  std::sort(opt_vec.begin(), opt_vec.end());
  opt_vec.erase(unique(opt_vec.begin(), opt_vec.end()),
                opt_vec.end());
  /* just so we don't get an unsigned int error */
  int opt_vec_size = opt_vec.size();
  if (verbose == true) {
    /* if we're being verbose then options used are listed */
    std::cout << "Options being used: " << std::endl;
    for (int man_ln = 0; man_ln < opt_vec_size;
         man_ln++) {
      std::cout << opt_vec.at(man_ln) << " ";
    }
    std::cout << std::endl;
  }
  if (dump_opts == true) {
    /* if we're going to dump the options of the manpage with -D
     * then just dump them out on STDOUT and be done with it!
     */
    for (int man_ln = 0; man_ln < opt_vec_size;
         man_ln++) {
      std::cout << opt_vec.at(man_ln) << std::endl;
    }
    std::cout << std::endl;
    exit(0);
  }
  /* return the options we've extracted out of the manpages */
  return (opt_vec);
#endif
#ifdef _WIN32
   /* just error out :( */
  std::cout << "Not implemented in Windows, sorry..." << std::endl;
  exit(1);
#endif
}
