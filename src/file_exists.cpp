#include <sys/stat.h>
#include <string>

bool file_exists(const std::string &filen) {
  struct stat buf;
  return (stat(filen.c_str(), &buf) == 0);
}

