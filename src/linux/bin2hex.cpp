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
  /* here we will initialize our variables */
  std::stringstream hex_out;
  std::string hexxy;
  /* set up the hex output with a filler of 0 in uppercase
   * hexadecimal with a width of two
   */
  hex_out << std::setw(2) << std::setfill('0') << std::hex
          << std::uppercase;
  /* for each of them go put the \\x infront for output to
   * printf, if we don't have anything in the buffer then
   * put two spaces
   */
  std::copy(bin_str.begin(), bin_str.end(),
            std::ostream_iterator<unsigned int>(
                hex_out, "\\\\x"));
  if (hex_out.str() != "") {
    hexxy = hex_out.str() + "20";
  }
  /* make sure that it begins in \\x for output */
  hexxy = "\\\\x" + hexxy;
  /* if the output is only spaces then return nothing */
  if (hexxy == "\\\\x20\\\\x20") {
    return "";
  } 
  /* return to sys_string in hex */
  else {
    return (hexxy);
  }
}
