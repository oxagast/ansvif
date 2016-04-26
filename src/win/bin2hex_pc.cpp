/*
 * ansvif
 * A Not So Very Intelligent Fuzzer
 * Marshall Whittaker / oxagast
 */

#include <string>
#include <sstream>
#include <iomanip>
#include <iterator>

std::string binstr_to_hex_pc(std::string bin_str) {
  std::stringstream hex_out;  // initialize the hex to go out
  std::string hexxy;          // initalize our string to put it in
  hex_out << std::setw(2) << std::setfill('0') << std::hex
          << std::uppercase;  // set the filler to 0, uppercase hexadecimal and
                              // width 2.
  std::copy(bin_str.begin(), bin_str.end(),
            std::ostream_iterator<unsigned int>(
                hex_out, "%"));  // for each of them, go put the % infront
                                     // for the output to go to printf
  if (hex_out.str() != "") {         // if we don't have anything in the buffer
    hexxy = hex_out.str() + "20";    // put a space
  }
  hexxy = "%" + hexxy;
  if (hexxy == "%20%20") {
    return "";
  }
  else {
    return (hexxy);  // return to sys_string in hex to be put into printf
  }

}
