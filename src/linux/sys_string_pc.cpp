/*
 * ansvif
 * A Not So Very Intelligent Fuzzer
 * Marshall Whittaker / oxagast
 */

//    __ _  _  __   ___  __  ____ ____ 
//   /  ( \/ )/ _\ / __)/ _\/ ___(_  _)
//  (  O )  (/    ( (_ /    \___ \ )(  
//   \__(_/\_\_/\_/\___\_/\_(____/(__)

#include <cstdlib>
#include <string>
#include <unistd.h>
#include <vector>

std::string binstr_to_hex(std::string bin_str);
std::string binstr_to_hex_pc(std::string bin_str_pc);
std::vector<std::string>
get_out_str_pc(std::string env_str, std::string valgrind_str,
               std::string sys_str, std::string path_str,
               std::string always_arg_b, std::string always_arg, 
               std::string fuzz_after, std::string log_prefix) {
  /* these are the strings that will go to be run in popen
   * out_str is normal, and out_str_p is pritnf compatible
   * for easy crash replay, except this is for % for
   * web browsers and such
   */
  std::string out_str, out_str_p;
  /* make sure we're not shooting blanks */
  if (sys_str != "") {
    if (env_str != "") {
      /* this is if we have an environment string */
      out_str_p =
          "$(printf \"" + binstr_to_hex(env_str) + "\") " + valgrind_str + " " +
          path_str + " $(printf \"" + binstr_to_hex(binstr_to_hex_pc(sys_str)) +
          "\") " + always_arg + binstr_to_hex(binstr_to_hex_pc(fuzz_after));
    }
    if (env_str == "") {
      /* and if we don't have environment variables to fuzz */
      out_str_p = valgrind_str + " " + path_str + " " + always_arg_b + 
                  " $(printf \"" + binstr_to_hex(binstr_to_hex_pc(sys_str)) + 
                  "\") " + always_arg + " $(printf \"" +
                  binstr_to_hex(binstr_to_hex_pc(fuzz_after)) +
                  "\") ";
    }
    out_str = env_str + " " + valgrind_str + " " + path_str + " " +
              always_arg_b + binstr_to_hex_pc(sys_str) + " " + always_arg +
              binstr_to_hex_pc(fuzz_after);
  }
  if (log_prefix == "") {
    /* incase we are logging don't leave a blank file */
    out_str = out_str + " >/dev/null 2>&1; echo $?";
  }
  else {
    /* get the signal here and log */
    out_str = out_str + " >" + log_prefix + ".output.ansvif.log 2>&1; echo $?";
  }
  /* we're putting the normal version and the printf
   * version in this vector, normal first, printf second,
   * then feeding it back to the calling routing
   */
  std::vector<std::string> out_all;
  out_all.push_back(out_str);
  out_all.push_back(out_str_p);
  return (out_all);
}
