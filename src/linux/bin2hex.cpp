#include <string>
#include <sstream>
#include <iomanip>
#include <iterator>

std::string binstr_to_hex(std::string bin_str) {
  std::stringstream hex_out;
  std::string hexxy;
  hex_out << std::setw(2) << std::setfill('0') << std::hex << std::uppercase;
  std::copy(bin_str.begin(), bin_str.end(),
            std::ostream_iterator<unsigned int>(hex_out, "\\\\x"));
  if (hex_out.str() != "") {
    hexxy = hex_out.str() + "20";
  }
  return (hexxy);
}
