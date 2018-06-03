#include <string>
#include <sys/stat.h>

bool file_exists(const std::string &filen) {
  struct stat buf;
  /* return if the file exists or not */
  return (stat(filen.c_str(), &buf) == 0);
}

