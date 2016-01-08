#include <string>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int toint(std::string ints) {
  return atoi(ints.c_str());  // for compatability with cygwin and mingw
}