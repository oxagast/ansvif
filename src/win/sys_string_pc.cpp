/*
 * ansvif
 * A Not So Very Intelligent Fuzzer
 * Marshall Whittaker / oxagast
 */

#include <string>
#include <vector>
#include <unistd.h>
#include <cstdlib>

std::string binstr_to_hex(std::string bin_str);
std::string binstr_to_hex_pc(std::string bin_str_pc);
std::vector<std::string> get_out_str_pc(std::string env_str,
                                     std::string valgrind_str,
                                     std::string sys_str, std::string path_str,
                                     std::string always_arg, std::string fuzz_after, std::string log_prefix) {
std::string out_str;
  std::string out_str_p;
  if (sys_str != "") {
    if (env_str != "") {
      out_str_p = " (.\\printf.exe \\x" + binstr_to_hex(env_str) + "\") " +
                  "'" + path_str + "' (.\\printf.exe \\x" +
                  binstr_to_hex(sys_str) + ")" + always_arg + " " + fuzz_after +
                  "; echo $LastExitCode";  // for windows compatibility
    }
    if (env_str == "") {
      out_str_p = "'" + path_str + "' (.\\printf.exe \\x" +
                  binstr_to_hex(sys_str) + ") " + always_arg + " " + fuzz_after +
                  "; echo $LastExitCode";  // also for win compatibility
    }
    out_str = env_str + "{& '" + path_str + "' " + sys_str + " " + always_arg + " " + fuzz_after;
  }
  out_str = out_str + " > " + log_prefix + ".output.ansvif.log; echo $LastExitCode}";  // get the signal
  std::vector<std::string> out_all;
  out_all.push_back(out_str);
  out_all.push_back(out_str_p);
  return (out_all);
}
