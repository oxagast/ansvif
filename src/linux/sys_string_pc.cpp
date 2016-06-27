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
               std::string always_arg, std::string fuzz_after,
               std::string log_prefix) {
  std::string out_str;   // this is for normal output (lots of garbage will show
                         // up, but normal characters)
  std::string out_str_p; // this is to be fed into printf on linux machines, a
                         // windows binary can be compiled from src to be
                         // compat with this
  if (sys_str != "") {   // if we're not blank on the sys side
    if (env_str != "") { // if we're not blank on the env side
      out_str_p =
          "$(printf \"" + binstr_to_hex(env_str) + "\") " + valgrind_str + " " +
          path_str + " $(printf \"" + binstr_to_hex(binstr_to_hex_pc(sys_str)) +
          "\") " + always_arg +
          binstr_to_hex(binstr_to_hex_pc(fuzz_after)); // put the printf in
                                                       // first but with a $(
                                                       // for running in
      // bash, then a hex space (space is added auto here
      // because it's 'blank') then turn the environment string
      // into hex, then close that out, then we'll put the
      // valgrind string, a space, the path to the program,
      // then do the same thing we did with the envionment
      // string with the sys string, and end it with the last
      // argument after the fuzz if we have one
    }
    if (env_str == "") { // if we have no environment string
      out_str_p = valgrind_str + " " + path_str + " $(printf \"" +
                  binstr_to_hex(binstr_to_hex_pc(sys_str)) + "\") " +
                  always_arg + " $(printf \"" +
                  binstr_to_hex(binstr_to_hex_pc(fuzz_after)) +
                  "\") "; // this is all the same, but without the
                          // environment string
    }
    out_str = env_str + " " + valgrind_str + " " + path_str + " " +
              binstr_to_hex_pc(sys_str) + " " + always_arg +
              binstr_to_hex_pc(fuzz_after); // this is just normal characters,
                                            // pretty easy, we have the
    // out_str being environment, valgrind if there is one
    // (which is handled elsewhere and just passed here), the
    // path string next, the system string, then whatever
    // argument is always meant to go after the fuzz
  }
  if (log_prefix == "") {
    out_str =
        out_str +
        "  >/dev/null 2>&1; echo $?"; // incase we're not logging so we don't
  }                                   // leave a blank file
  else {
    out_str = out_str + ">" + log_prefix +
              ".output.ansvif.log 2>&1; echo $?"; // get the signal
  }
  std::vector<std::string> out_all; // initialize the vector to put out_str and
                                    // out_str_p (regular and printf version)
                                    // in
  out_all.push_back(out_str);       // put regular version in first
  out_all.push_back(out_str_p);     // put printf version in last
  return (out_all);                 // return it for the rest of the program
} // we're done now
