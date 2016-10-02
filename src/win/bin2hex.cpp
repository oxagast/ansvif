/*
 * ansvif
 * A Not So Very Intelligent Fuzzer
 * Marshall Whittaker / oxagast
 */

//    __ _  _  __   ___  __  ____ ____ 
//   /  ( \/ )/ _\ / __)/ _\/ ___(_  _)
//  (  O )  (/    ( (_ /    \___ \ )(  
//   \__(_/\_\_/\_/\___\_/\_(____/(__)

#include <iomanip>
#include <iterator>
#include <sstream>
#include <string>

std::string binstr_to_hex(std::string bin_str) {
  /* initialize the hex to go out */
  std::stringstream hex_out;
  /* initialize our string to put it in with a filler of
   * zero and upercase hexadecimal with a width of 2
   */
  std::string hexxy;
  hex_out << std::setw(2) << std::setfill('0') << std::hex
          << std::uppercase;
  std::copy(bin_str.begin(), bin_str.end(),
            std::ostream_iterator<unsigned int>(
                hex_out, "\\x"));
  if (hex_out.str() != "") {
    /* if we don't have anything in the buffer throw
     * a space in
     */
    hexxy = hex_out.str() + "20";
  }
  hexxy = "\\x" + hexxy;
  if (hexxy == "\\x20\\x20") {
    return "";
  } else {
    /* return to be fed into printf */
    return (hexxy);
  }
}
