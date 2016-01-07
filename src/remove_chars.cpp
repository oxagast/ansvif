#include <unistd.h>
#include <string>

std::string remove_chars(const std::string &source, const std::string &chars) {
  std::string result = "";
  for (unsigned int i = 0; i < source.length(); i++) {
    bool foundany = false;
    for (unsigned int j = 0; j < chars.length() && !foundany; j++) {
      foundany = (source[i] == chars[j]);
    }
    if (!foundany) {
      result += source[i];
    }
  }
  return (result);
}

