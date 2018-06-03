#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>
#ifdef __unix
#include "src/version.h"
#endif
#ifdef _WIN32
#include "version.h"
#endif
#include <sstream>

void help_me(std::string mr_me);

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
  return (0);
}
 
