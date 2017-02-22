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
std::vector<std::string>
get_out_str(std::string env_str, std::string valgrind_str, std::string sys_str,
            std::string path_str, std::string always_arg_b, std::string always_arg,
            std::string fuzz_after, std::string log_prefix) {
  /* these are the strings that will go to be run in popen
   * out_str is normal, and out_str_p is pritnf compatible
   * for easy crash replay, except this is for % for
   * web browsers and such
   */
  std::string out_str, out_str_p;
  /* no shooting blanks plz */
  if (sys_str != "") {
    if (env_str != "") {
      out_str_p =
          "$(printf \"" + binstr_to_hex(env_str) + "\") " + valgrind_str + " " +
          path_str + " " + always_arg_b + " $(printf \"" + binstr_to_hex(sys_str) + "\") " +
          always_arg +
          fuzz_after;
    }
    if (env_str == "") { // if we have no environment string
      out_str_p = valgrind_str + " " + path_str + " " + always_arg_b + " $(printf \"" +
                  binstr_to_hex(sys_str) + "\") " + always_arg +
                  " $(printf \"" + binstr_to_hex(fuzz_after) +
                  "\") ";
    }
    out_str =
        env_str + " " + valgrind_str + " " + path_str + " " +
        always_arg_b + sys_str + " " +
        always_arg + fuzz_after;
  }
  if (log_prefix == "") {
    /* not logging here */
    out_str =
        out_str +
        "  >/dev/null 2>&1; echo magic_token_CRASHCODE $?";
  }
  else {
    /* logging here */
    out_str = out_str + " >" + log_prefix +
              ".output.ansvif.log 2>&1; echo magic_token_CRASHCODE $?";
  }
  /* here we declare out_all and put the out_str and out_str_p
   * printf compatible stuff into the vector to be fed back into
   * the calling routine
   */
  std::vector<std::string> out_all;
  out_all.push_back(out_str);
  out_all.push_back(out_str_p);
  return (out_all);
}
