#include <string>
#include <vector>
#include <unistd.h>
#include <cstdlib>

std::string binstr_to_hex(std::string bin_str);
std::vector<std::string> get_out_str(std::string env_str,
                                     std::string valgrind_str,
                                     std::string sys_str, std::string path_str,
                                     std::string always_arg) {
  std::string out_str;
  std::string out_str_p;
  if (sys_str != "") {
    if (env_str != "") {
      out_str_p = "$(printf \"\\\\x" + binstr_to_hex(env_str) + "\") " +
                  valgrind_str + " " + path_str + " $(printf \"\\\\x" +
                  binstr_to_hex(sys_str) + "\") " + always_arg;
    }
    if (env_str == "") {
      out_str_p = valgrind_str + " " + path_str + " $(printf \"\\\\x" +
                  binstr_to_hex(sys_str) + "\") " + always_arg;
    }
    out_str = env_str + " " + valgrind_str + " " + path_str + " " + sys_str +
              " " + always_arg;
  }
  out_str = out_str + "; echo $?";  // get the signal
  std::vector<std::string> out_all;
  out_all.push_back(out_str);
  out_all.push_back(out_str_p);
  return (out_all);
}
