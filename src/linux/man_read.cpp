/*
 * ansvif
 * A Not So Very Intelligent Fuzzer
 * Marshall Whittaker / oxagast
 */

#include "../../include/gzstream/gzstream.h"
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
  std::string filename;
  std::vector<std::string> opt_vec;
  filename = "/usr/share/man/man" + man_loc + "/" + man_page + "." + man_loc +
             ".gz"; // put the filename back together
  if (file_exists(filename) == true) {
    char *chr_fn =
        strdup(filename.c_str()); // change the filename to a char pointer
    igzstream in(chr_fn);         //  gunzip baby
    std::string gzline;           // initialize a line out of the gzstream
    std::regex start_of_opt_1(
        "^(\\.?\\\\?\\w{2} )*(\\\\?\\w{2} ?)*(:?\\.B "
        ")*((?:(?:\\\\-)+\\w+)(?:\\\\-\\w+)*).*"); // hella regex... why you do
                                                   // this to me manpage?
    std::smatch opt_part_1;                        // we'll match now
    std::regex start_of_opt_2(
        "^\\.Op Fl (\\w+) Ar.*"); // also a stupid fucking regex
    std::smatch opt_part_2;       // try to match again
    std::regex start_of_opt_3("^\\\\fB(-.*)\\\\fB.*"); // ANOTHER goddamn regex
                                                       // for the manpage --
                                                       // fml
    std::smatch opt_part_3;            // try to match the theird regex
    while (std::getline(in, gzline)) { // if we've got a manpage...
      if (std::regex_match(gzline, opt_part_1,
                           start_of_opt_1)) { // ring 'er out
        std::string opt_1 = opt_part_1[4];    // initialize opt_1 as a string
        std::string opt_release = (remove_chars(
            opt_part_1[4], "\\"));      // remove the fucking backslashes plz
        opt_vec.push_back(opt_release); // push the stuff into opt vec after we
                                        // moved the excessive backslashes
      }
      if (std::regex_match(
              gzline, opt_part_2,
              start_of_opt_2)) { // if we've got a different style manpage...
        std::string opt_2 = opt_part_2[1]; // initialize opt two
        opt_vec.push_back("-" + opt_2);    // push the opt onto option vector
      }
      if (std::regex_match(
              gzline, opt_part_3,
              start_of_opt_3)) {           // if we have the third style manpage
        std::string opt_3 = opt_part_3[1]; // initialize opt three
        opt_vec.push_back(opt_3);          // push the opt into option vector
      }
    }
  } else {
    std::cerr << "Could not find a manpage for that command..."
              << std::endl; // say that we couldn't find the manpage because
                            // either they didn't enter the right location, or
                            // the command doesn't have one
    exit(1);                // exit on error 1
  }
  std::sort(opt_vec.begin(),
            opt_vec.end()); // sort 'em first so that unique will work correctly
  opt_vec.erase(unique(opt_vec.begin(), opt_vec.end()),
                opt_vec.end());      // then we'll run unique over the vector
                                     // holding all the options
  int opt_vec_size = opt_vec.size(); // so we don't get a error on compliation
                                     // about unsigned compared with int
  if (verbose == true) {             // if we're talkative today...
    std::cout << "Options being used: "
              << std::endl; // start telling them what options will be used...
    for (int man_ln = 0; man_ln < opt_vec_size;
         man_ln++) {                          // loop around the options
      std::cout << opt_vec.at(man_ln) << " "; // output options for the user
    }
    std::cout << std::endl; // end the line, duh
  }
  if (dump_opts == true) { // if we've got the dump options flag set then we're
                           // going to JUST dump all the options from the
                           // manpage to STDOUT and be done with it
    for (int man_ln = 0; man_ln < opt_vec_size;
         man_ln++) {                                // loop around the options
      std::cout << opt_vec.at(man_ln) << std::endl; // output options only
    }
    std::cout << std::endl; // end the line
    exit(0);                // exit cleanly
  }
  return (opt_vec); // or return the options we've extracted out of the manpages
}
