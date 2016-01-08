#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>

std::string remove_chars(const std::string &source, const std::string &chars);
bool file_exists(const std::string &filen);

std::vector<std::string> get_flags_man(char *cmd, std::string man_loc,
                                       bool verbose, bool debug,
                                       bool dump_opts) {
  std::cout << "Not implemented in Windows, sorry..." << std::endl;
  exit(1);
}
