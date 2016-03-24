#include <regex>
#include "../../include/gzstream/gzstream.h"
#include <thread>
#include <vector>
#include <unistd.h>
#include <cstring>

std::string remove_chars(const std::string &source, const std::string &chars);
bool file_exists(const std::string &filen);

std::vector<std::string> get_flags_man(std::string man_page,
                                       std::string man_loc, bool verbose,
                                       bool debug, bool dump_opts) {
  std::string filename;
  std::vector<std::string> opt_vec;
  filename = "/usr/share/man/man" + man_loc + "/" + man_page + "." + man_loc +
             ".gz";  // put the filename back together
  if (file_exists(filename) == true) {
    char *chr_fn =
        strdup(filename.c_str());  // change the filename to a char pointer
    igzstream in(chr_fn);          //  gunzip baby
    std::string gzline;
    std::regex start_of_opt_1(
        "^(\\.?\\\\?\\w{2} )*(\\\\?\\w{2} ?)*(:?\\.B "
        ")*((?:(?:\\\\-)+\\w+)(?:\\\\-\\w+)*).*");  // hella regex... why you do
                                                    // this to me manpage?
    std::smatch opt_part_1;
    std::regex start_of_opt_2("^\\.Op Fl (\\w+) Ar.*");
    std::smatch opt_part_2;
    std::regex start_of_opt_3("^\\\\fB(-.*)\\\\fB.*");
    std::smatch opt_part_3;
    while (std::getline(in, gzline)) {
      if (std::regex_match(gzline, opt_part_1,
                           start_of_opt_1)) {  // ring 'er out
        std::string opt_1 = opt_part_1[4];
        std::string opt_release = (remove_chars(
            opt_part_1[4], "\\"));  // remove the fucking backslashes plz
        opt_vec.push_back(opt_release);
      }
      if (std::regex_match(gzline, opt_part_2, start_of_opt_2)) {
        std::string opt_2 = opt_part_2[1];
        opt_vec.push_back("-" + opt_2);
      }
      if (std::regex_match(gzline, opt_part_3, start_of_opt_3)) {
        std::string opt_3 = opt_part_3[1];
        opt_vec.push_back(opt_3);
      }
    }
  } else {
    std::cerr << "Could not find a manpage for that command..." << std::endl;
    exit(1);
  }
  std::sort(opt_vec.begin(), opt_vec.end());
  opt_vec.erase(unique(opt_vec.begin(), opt_vec.end()), opt_vec.end());
  int opt_vec_size = opt_vec.size();
  if (verbose == true) {
    std::cout << "Options being used: " << std::endl;
    for (int man_ln = 0; man_ln < opt_vec_size;
         man_ln++) {                           // loop around the options
      std::cout << opt_vec.at(man_ln) << " ";  // output options
    }
    std::cout << std::endl;
  }
  if (dump_opts == true) {
    for (int man_ln = 0; man_ln < opt_vec_size;
         man_ln++) {                                 // loop around the options
      std::cout << opt_vec.at(man_ln) << std::endl;  // output options
    }
    std::cout << std::endl;
    exit(0);
  }
  return (opt_vec);
}
