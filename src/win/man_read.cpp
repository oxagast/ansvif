/*
 * ansvif
 * A Not So Very Intelligent Fuzzer
 * Marshall Whittaker / oxagast
 */

#include <cstdlib>
#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>

std::string remove_chars(const std::string &source, const std::string &chars);
bool file_exists(const std::string &filen);

std::vector<std::string> get_flags_man(std::string man_page,
                                       std::string man_loc, bool verbose,
                                       bool debug, bool dump_opts) {
  std::cout << "Not implemented in Windows, sorry..." << std::endl;
  exit(1);
}
