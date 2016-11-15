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

std::string binstr_to_hex_pc(std::string bin_str) {
  /* initialize our variables */    
  std::stringstream hex_out;
  std::string hexxy;
  /* set the filler to 0 in uppercase hex and width to 2 */
  hex_out << std::setw(2) << std::setfill('0') << std::hex
          << std::uppercase;
  
  /* for each character put the % infront for the output of
   * printf if we don't have anything in the buffer we'll
   * put a space
   */        
  std::copy(bin_str.begin(), bin_str.end(),
    std::ostream_iterator<unsigned int>(hex_out, "%"));
  if (hex_out.str() != "") {
    hexxy = hex_out.str() + "20";
  }
  /* make sure it starts with a % in this case */
  hexxy = "%" + hexxy;
  /* if nothing is in it but hex spaces then we will
   * return nothing
   */
  if (hexxy.substr(0,3) == "%20") {
    return "";
  } 
  /* if there is stuff in the variable the return sys_string
   * in hex
   */
  else {
    return (hexxy);
  }
}
