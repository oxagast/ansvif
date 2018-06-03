#include <cstdlib>
#include <string>
#include <unistd.h>

std::string remove_chars(const std::string &source, const std::string &chars) {
  // initialize an empty removal string //
        std::string result = "";
  for (unsigned int i = 0; i < source.length(); i++) {
    // loop through each character removing the ones we
    // don't want
    //
    bool foundany = false;
    for (unsigned int j = 0; j < chars.length() && !foundany; j++) {
      foundany = (source[i] == chars[j]);
    }
    if (!foundany) {
      // put it back in the buffer if it's not in the list
      result += source[i];
    }
  }
  return (result);
}
