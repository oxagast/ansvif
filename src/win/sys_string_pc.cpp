/*
 * ansvif
 * A Not So Very Intelligent Fuzzer
 * Marshall Whittaker / oxagast
 */

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
  std::string out_str;
  std::string out_str_p;
  if (sys_str != "") {
    if (env_str != "") {
      out_str_p = " (.\\printf.exe \\x" + binstr_to_hex(env_str) + "\") " +
                  "'" + path_str + "' (.\\printf.exe \\x" +
                  binstr_to_hex(sys_str) + ")" + always_arg + " " + fuzz_after +
                  "; echo $LastExitCode";
    }
    if (env_str == "") {
      out_str_p = "'" + path_str + "' (.\\printf.exe \\x" +
                  binstr_to_hex(sys_str) + ") " + always_arg + " " +
                  fuzz_after +
                  "; echo $LastExitCode";
    }
   out_str = "powershell -c " + env_str + "(Start-Job {& '" + path_str + "' " + sys_str + " " +
              always_arg + " " + fuzz_after;
  }
  out_str = out_str + " > " + log_prefix +
            ".output.ansvif.log; echo $LastExitCode}";
  std::vector<std::string> out_all;
  out_all.push_back(out_str);
  out_all.push_back(out_str_p);
  return (out_all);
}
