
/*
 * ansvif
 * A Not So Very Intelligent Fuzzer
 * Marshall Whittaker / oxagast
 */

#include <regex>
#include "../include/gzstream/gzstream.h"
#include <thread>
#include <iomanip>
#include <sys/wait.h>
#include <sys/stat.h>
#include <vector>

#define READ 0
#define WRITE 1

void help_me(std::string mr_me) {
  std::cout
      << "Usage:" << std::endl
      << " " << mr_me << " -t template -c ./faulty -b 2048" << std::endl
      << "Options:" << std::endl
      << " -t [file]    This file should hold line by line command arguments"
      << std::endl
      << "              as shown in the example file." << std::endl
      << " -e [file]    This file should hold line by line environment "
         "variables"
      << std::endl
      << "              as shown in the example file.  You can" << std::endl
      << "              usually get these by doing something like:" << std::endl
      << "              $ strings /bin/mount | perl -ne 'print if /[A-Z]=$/' > "
         "mount_envs"
      << std::endl
      << " -c [path]    Specifies the command path." << std::endl
      << " -p [integer] Specifies the manpage location (as an integer, usually "
         "1 or 8)."
      << std::endl
      << " -m [command] Specifies the commands manpage." << std::endl
      << " -D           Dump what we can out of the manpage to stdout."
      << std::endl
      << " -f [integer] Number of threads to use.  Default is 2." << std::endl
      << " -b [integer] Specifies the buffer size to fuzz with." << std::endl
      << "              256-2048 Is usually sufficient." << std::endl
      << " -r           Use only random data." << std::endl
      << " -z           Randomize buffer size from 1 to specified by the -b "
         "option."
      << std::endl
      << " -s \"@#^$CE\"  Characters to omit from randomization.  Default "
         "omitted"
      << std::endl
      << "              characters are: <>\\n |&\[]\()\{}:; and mandatory "
         "omitted"
      << std::endl
      << "              characters are: >\\n" << std::endl
      << " -o [file]    Log to this file." << std::endl
      << " -x [file]    Other opts to put in, such as usernames, etc."
      << std::endl
      << " -S \",\"       Some seperator besides 'space' between opts, such as "
         "',:-' etc."
      << std::endl
      << " -L [nobody]  An unprivileged user to run as if you're root.  "
         "Defaults nobody."
      << std::endl
      << " -A \"blah\"    Always put this string in the command." << std::endl
      << " -F [file]    A file with junk to be fuzzed with whole." << std::endl
      << " -n           Never use random data in the fuzz." << std::endl
      << " -R \"ls\"      Always run this command after the fuzz." << std::endl
      << " -W [integer] Thread timeout." << std::endl
      << " -v           Verbose." << std::endl
      << " -d           Debug." << std::endl;
  exit(0);
}

std::string remove_chars(const std::string &source, const std::string &chars) {
  std::string result = "";
  for (unsigned int i = 0; i < source.length(); i++) {
    bool foundany = false;
    for (unsigned int j = 0; j < chars.length() && !foundany; j++) {
      foundany = (source[i] == chars[j]);
    }
    if (!foundany) {
      result += source[i];
    }
  }
  return (result);
}

bool file_exists(const std::string &filen) {
  struct stat buf;
  return (stat(filen.c_str(), &buf) == 0);
}

int rand_me_plz(int rand_from, int rand_to) {
  std::random_device rd;
  std::default_random_engine generator(rd()); // seed random
  std::uniform_int_distribution<int> distribution(rand_from,
                                                  rand_to); // get a random
  auto roll = std::bind(distribution,
                        generator); // bind it so we can do it multiple times
  return (roll());
}

char fortune_cookie() {
  char chr;
  const char *hex_digits = "0123456789ABCDEF";
  int i;
  for (i = 0; i < 1; i++) {
    chr = hex_digits[(rand_me_plz(0, 255))];
  }
  return (chr);
}

int reaper(int c_pid, int t_timeout) {
  //  std::this_thread::sleep_for(std::chrono::milliseconds(t_timeout));
  sleep(t_timeout);
  kill(c_pid, 9);
  return (0);
}

std::vector<std::string> get_flags_man(char *cmd, std::string man_loc,
                                       bool verbose, bool debug,
                                       bool dump_opts) {
  std::string cmd_name(cmd);
  std::string filename;
  std::vector<std::string> opt_vec;
  filename = "/usr/share/man/man" + man_loc + "/" + cmd_name + "." + man_loc +
             ".gz"; // put the filename back together
  if (file_exists(filename) == true) {
    char *chr_fn =
        strdup(filename.c_str()); // change the filename to a char pointer
    igzstream in(chr_fn);         //  gunzip baby
    std::string gzline;
    std::regex start_of_opt_1(
        "^(\\.?\\\\?\\w{2} )*(\\\\?\\w{2} ?)*(:?\\.B "
        ")*((?:(?:\\\\-)+\\w+)(?:\\\\-\\w+)*).*"); // hella regex... why you do
                                                   // this to me manpage?
    std::smatch opt_part_1;
    std::regex start_of_opt_2("^\\.Op Fl (\\w+) Ar.*");
    std::smatch opt_part_2;
    std::regex start_of_opt_3("^\\\\fB(-.*)\\\\fB.*");
    std::smatch opt_part_3;
    while (std::getline(in, gzline)) {
      if (std::regex_match(gzline, opt_part_1,
                           start_of_opt_1)) { // ring 'er out
        std::string opt_1 = opt_part_1[4];
        std::string opt_release = (remove_chars(
            opt_part_1[4], "\\")); // remove the fucking backslashes plz
        opt_vec.push_back(opt_release);
      }
      if (std::regex_match(gzline, opt_part_2, start_of_opt_2)) {
        std::string opt_2 = opt_part_2[1];
        opt_vec.push_back("-" + opt_2);
      }
      if (std::regex_match(gzline, opt_part_3, start_of_opt_3)) {
        std::string opt_3 = opt_part_3[1];
        opt_vec.push_back(opt_3);
      }
    }
  } else {
    std::cerr << "Could not find a manpage for that command..." << std::endl;
    exit(1);
  }
  std::sort(opt_vec.begin(), opt_vec.end());
  opt_vec.erase(unique(opt_vec.begin(), opt_vec.end()), opt_vec.end());
  if (verbose == true) {
    std::cout << "Options being used: " << std::endl;
    for (int man_ln = 0; man_ln < opt_vec.size();
         man_ln++) {                          // loop around the options
      std::cout << opt_vec.at(man_ln) << " "; // output options
    }
    std::cout << std::endl;
  }
  if (dump_opts == true) {
    for (int man_ln = 0; man_ln < opt_vec.size();
         man_ln++) {                                // loop around the options
      std::cout << opt_vec.at(man_ln) << std::endl; // output options
    }
    std::cout << std::endl;
    exit(0);
  }
  return (opt_vec);
}

std::vector<std::string> get_flags_template(std::string filename, bool verbose,
                                            bool debug) {
  std::vector<std::string> opt_vec;
  std::string line;
  std::ifstream template_file(filename);
  if (template_file.is_open()) {
    while (std::getline(template_file, line)) {
      opt_vec.push_back(line);
    }
    template_file.close();
  } else {
    std::cerr << "Could not open template file..." << std::endl;
    exit(1);
  }
  return (opt_vec);
}

std::vector<std::string> get_other(std::string filename, bool verbose,
                                   bool debug) {
  std::vector<std::string> other_vec;
  std::string line;
  std::ifstream other_file(filename);
  if (other_file.is_open()) {
    while (std::getline(other_file, line)) {
      other_vec.push_back(line);
    }
    other_file.close();
  } else {
    std::cerr << "Could not open template file..." << std::endl;
    exit(1);
  }
  return (other_vec);
}

std::string trash_generator(int trash, int buf, std::string user_junk,
                            std::string opt_other_str, bool never_rand) {
  std::string junk = "";
  std::string hex_stuff;
  int trash_num;
  if (trash == 0) { // kosher
    for (trash_num = 0; trash_num < buf; trash_num++) {
      junk = "A" + junk; // put lots of As
    }
  }
  if (trash == 1) {
    for (trash_num = 0; trash_num < buf; trash_num++) {
      junk = "9" + junk; // yadda yadda
    }
  }
  if (trash == 2) {
    for (trash_num = 0; trash_num < buf; trash_num++) {
      junk = "\%s" + junk; // yadda yadda
    }
  }
  if (never_rand == false) {
    if (trash == 3) {
      for (trash_num = 0; trash_num < buf; trash_num++) {
        junk = junk += fortune_cookie();
      }
    }
  }
  if (trash == 4) { // front
    for (trash_num = 0; trash_num < buf; trash_num++) {
      junk = "A" + junk; // put lots of As
    }
    junk = user_junk + junk;
    if (buf - user_junk.length() < junk.size())
      junk = junk.substr(0, buf);
    else
      return ("OOR");
  }
  if (trash == 5) {
    for (trash_num = 0; trash_num < buf; trash_num++) {
      junk = "9" + junk; // yadda yadda
    }
    junk = user_junk + junk;
    if (buf - user_junk.length() < junk.size())
      junk = junk.substr(0, buf);
    else
      return ("OOR");
  }
    if (trash == 6) {
    for (trash_num = 0; trash_num < buf; trash_num++) {
      junk = "\%s" + junk; // yadda yadda
    }
    junk = user_junk + junk;
    if (buf - user_junk.length() < junk.size())
      junk = junk.substr(0, buf);
    else
      return ("OOR");
  }
  if (never_rand == false) {
    if (trash == 7) {
      for (trash_num = 0; trash_num < buf; trash_num++) {
        junk = junk += fortune_cookie();
      }
      junk = user_junk + junk;
      if (buf - user_junk.length() < junk.size())
        junk = junk.substr(0, buf);
      else
        return ("OOR");
    }
  }
  if (trash == 8) {
    for (trash_num = 0; trash_num < buf; trash_num++) { // back
      junk = "A" + junk;                                // put lots of As
    }
    junk = junk + user_junk;
    if (buf - user_junk.length() < junk.size())
      junk = junk.substr(junk.length() - buf);
    else
      return ("OOR");
  }
  if (trash == 9) {
    for (trash_num = 0; trash_num < buf; trash_num++) {
      junk = "9" + junk; // yadda yadda
    }
    junk = junk + user_junk;
    if (buf - user_junk.length() < junk.size())
      junk = junk.substr(junk.length() - buf);
    else
      return ("OOR");
  }
  if (trash == 10) {
    for (trash_num = 0; trash_num < buf; trash_num++) {
      junk = "\%s" + junk; // yadda yadda
    }
    junk = junk + user_junk;
    if (buf - user_junk.length() < junk.size())
      junk = junk.substr(junk.length() - buf);
    else
      return ("OOR");
  }
  if (never_rand == false) {
    if (trash == 11) {
      for (trash_num = 0; trash_num < buf; trash_num++) {
        junk = junk += fortune_cookie();
      }
      junk = junk + user_junk;
      if (buf - user_junk.length() < junk.size())
        junk = junk.substr(junk.length() - buf);
      else
        return ("OOR");
    }
  }
  if (trash == 12) {
    junk = opt_other_str;
  }
  return (junk);
}

std::string make_garbage(int trash, int buf, std::string opt_other_str,
                         bool is_other, bool never_rand) {
  buf = buf - 1;
  std::string all_junk;
  if (is_other == true) {
    if (isatty(STDIN_FILENO)) {
      std::string user_stuff = "";
      all_junk =
          trash_generator(trash, buf, user_stuff, opt_other_str, never_rand);
    } else {
      std::string user_stuff;
      getline(std::cin, user_stuff);
      all_junk =
          trash_generator(trash, buf, user_stuff, opt_other_str, never_rand);
    }
  } else if (is_other == false) {
    if (isatty(STDIN_FILENO)) {
      std::string user_stuff = "";
      all_junk = trash_generator(trash, buf, user_stuff, "", never_rand);
    } else {
      std::string user_stuff;
      getline(std::cin, user_stuff);
      all_junk = trash_generator(trash, buf, user_stuff, "", never_rand);
    }
  }
  return (all_junk);
}

FILE *popen2(std::string command, std::string type, int &pid,
             std::string low_lvl_user) {
  pid_t child_pid;
  int fd[1];
  pipe(fd);
  if ((child_pid = fork()) == -1) {
    perror("fork");
    exit(1);
  }
  if (child_pid == 0) { // child begins
    if (type == "r") {
      close(fd[READ]); // Close the READ
      dup2(fd[WRITE], 1); // Redirect stdout to pipe
    } else {
      close(fd[WRITE]); // Close the WRITE
      dup2(fd[READ], 0); // Redirect stdin to pipe
    }
    if (getuid() == 0) {
      execl("/bin/su", "su", "-c", "/bin/sh", "-c", command.c_str(),
            low_lvl_user.c_str(),
            NULL); // fixes not being able to reap suid 0 processes
    } else {
      execl("/bin/sh", "/bin/sh", "-c", command.c_str(), NULL); // runs it all
    }
    exit(0);
  } else {
    if (type == "r") {
      close(fd[WRITE]); // Close the WRITE
    } else {
      close(fd[READ]); // Close the READ
    }
  }
  pid = child_pid;
  if (type == "r") {
    return fdopen(fd[READ], "r");
  }
  return fdopen(fd[WRITE], "w");
}

int pclose2(FILE *fp, pid_t pid) // close it so we don't fuck outselves
{
  int stat;
  fclose(fp);
  while (waitpid(pid, &stat, 0) == 0) {
    if (errno != EINTR) {
      stat = -1;
      break;
    }
  }
  return stat;
}

std::string binstr_to_hex(std::string bin_str) {
  std::stringstream hex_out;
  std::string hexxy;
  hex_out << std::setw(2) << std::setfill('0') << std::hex << std::uppercase;
  std::copy(bin_str.begin(), bin_str.end(),
            std::ostream_iterator<unsigned int>(hex_out, "\\\\x"));
  if (hex_out.str() != "") {
    hexxy = hex_out.str() + "20";
  }
  return (hexxy);
}

void write_seg(std::string filename, std::string seg_line) {
  std::ofstream w_f;
  w_f.open(filename, std::ios::out | std::ios::app);
  w_f << seg_line << std::endl;
  w_f.close();
}

void write_junk_file(std::string filename, std::vector<std::string> opt_other,
                     int buf_size, int rand_spec_one, int rand_spec_two,
                     bool never_rand, std::string other_sep, bool verbose) {
  remove(filename.c_str());
  std::string oscar;
  std::ofstream w_f;
  w_f.open(filename, std::ios::out | std::ios::app);
  for (int start_buf = 0; start_buf <= buf_size; start_buf++) {
    std::string oscar = opt_other.at(rand_me_plz(0, opt_other.size() - 1));
    std::string trash =
        make_garbage(rand_me_plz(rand_spec_one, rand_spec_two),
                     rand_me_plz(1, buf_size), "", false, never_rand);
    w_f << oscar;
    if (trash != "OOR") {
      w_f << trash;
    }
    int is_sep = rand_me_plz(0, 1);
    if (is_sep == 1) {
      w_f << other_sep << std::endl;
    } else {
      w_f << std::endl;
    }
    if (verbose == true) {
      std::cerr << oscar;
      {
        if (trash != "OOR")
          std::cerr << trash;
      }
      if (is_sep == 1) {
        std::cerr << other_sep << std::endl;
      } else {
        std::cerr << std::endl;
      }
    }
  }
  w_f.close();
}

bool match_seg(int buf_size, std::vector<std::string> opts,
               std::vector<std::string> spec_env, std::string path_str,
               std::string strip_shell, bool rand_all, bool write_to_file,
               std::string write_file_n, bool rand_buf,
               std::vector<std::string> opt_other, bool is_other,
               std::string other_sep, int t_timeout, std::string low_lvl_user,
               std::string junk_file_of_args, std::string always_arg,
               bool never_rand, std::string run_command, bool verbose,
               bool debug) {
  bool segged = false;
  if (file_exists(path_str) == true) {
    while (segged == false) {
      int rand_spec_one, rand_spec_two;
      if (rand_all == true) {
        rand_spec_one = 2;
        rand_spec_two = 2;
      } else {
        rand_spec_one = 0;
        rand_spec_two = 12;
      }
      std::vector<std::string> junk_opts_env;
      std::vector<std::string> junk_opts;
      std::string env_str;
      std::string sys_str;
      if (junk_file_of_args != "") {
        write_junk_file(junk_file_of_args, opt_other, buf_size, rand_spec_one,
                        rand_spec_two, never_rand, other_sep, verbose);
      }
      int sep_type;
      for (int cmd_flag_l = 0; cmd_flag_l < opts.size();
           cmd_flag_l++) {            // loop around the options
        if (rand_me_plz(0, 1) == 1) { // roll tha die
          junk_opts.push_back(
              opts.at(cmd_flag_l)); // put the random arg in the vector
        }
      }
      for (int cmd_flag_a = 0; cmd_flag_a < spec_env.size();
           cmd_flag_a++) {            // loop around the options
        if (rand_me_plz(0, 1) == 1) { // roll tha die
          junk_opts_env.push_back(
              spec_env.at(cmd_flag_a)); // put the random arg in the vector
        }
      }
      if (is_other == true) {
        if (rand_buf == true) {
          for (std::vector<std::string>::const_iterator junk_opt_env =
                   junk_opts_env.begin();
               junk_opt_env != junk_opts_env.end();
               ++junk_opt_env) { // loop through the vector of junk envs
            std::string oscar_env = remove_chars(
                make_garbage(rand_me_plz(rand_spec_one, rand_spec_two),
                             rand_me_plz(1, buf_size),
                             opt_other.at(rand_me_plz(0, opt_other.size() - 1)),
                             is_other, never_rand),
                " ");
            if (oscar_env != "OOR") {
              env_str = env_str + *junk_opt_env + oscar_env + " ";
            }
          }
          for (std::vector<std::string>::const_iterator junk_opt =
                   junk_opts.begin();
               junk_opt != junk_opts.end();
               ++junk_opt) { // loop through the vector of junk opts
            std::string oscar = remove_chars(
                make_garbage(rand_me_plz(rand_spec_one, rand_spec_two),
                             rand_me_plz(1, buf_size),
                             opt_other.at(rand_me_plz(0, opt_other.size() - 1)),
                             is_other, never_rand),
                strip_shell);
            if (oscar != "OOR") {
              sep_type = rand_me_plz(0, 1);
              if (sep_type == 0) {
                sys_str = " " + sys_str + *junk_opt + " " + oscar + " ";
              }
              if (sep_type == 1) {
                sys_str =
                    sys_str + " " + *junk_opt + other_sep + oscar + other_sep;
              }
            }
          }
        } else if (rand_buf == false) {
          for (std::vector<std::string>::const_iterator junk_opt_env =
                   junk_opts_env.begin();
               junk_opt_env != junk_opts_env.end();
               ++junk_opt_env) { // loop through the vector of junk envs
            std::string oscar_env = remove_chars(
                make_garbage(rand_me_plz(rand_spec_one, rand_spec_two),
                             buf_size,
                             opt_other.at(rand_me_plz(0, opt_other.size() - 1)),
                             is_other, never_rand),
                " ");
            if (oscar_env != "OOR") {
              env_str = env_str + *junk_opt_env + oscar_env + " ";
            }
          }
          for (std::vector<std::string>::const_iterator junk_opt =
                   junk_opts.begin();
               junk_opt != junk_opts.end();
               ++junk_opt) { // loop through the vector of junk opts
            std::string oscar = remove_chars(
                make_garbage(rand_me_plz(rand_spec_one, rand_spec_two),
                             buf_size,
                             opt_other.at(rand_me_plz(0, opt_other.size() - 1)),
                             is_other, never_rand),
                strip_shell);
            if (oscar != "OOR") {
              sep_type = rand_me_plz(0, 1);
              if (sep_type == 0) {
                sys_str = " " + sys_str + *junk_opt + " " + oscar + " ";
              }
              if (sep_type == 1) {
                sys_str =
                    sys_str + " " + *junk_opt + other_sep + oscar + other_sep;
              }
            }
          }
        }
      }
      if (is_other == false) {
        if (rand_buf == true) {
          for (std::vector<std::string>::const_iterator junk_opt_env =
                   junk_opts_env.begin();
               junk_opt_env != junk_opts_env.end();
               ++junk_opt_env) { // loop through the vector of junk envs
            std::string oscar_env = remove_chars(
                make_garbage(rand_me_plz(rand_spec_one, rand_spec_two),
                             rand_me_plz(1, buf_size), "", is_other,
                             never_rand),
                " ");
            if (oscar_env != "OOR") {
              env_str = env_str + *junk_opt_env + oscar_env + " ";
            }
          }
          for (std::vector<std::string>::const_iterator junk_opt =
                   junk_opts.begin();
               junk_opt != junk_opts.end();
               ++junk_opt) { // loop through the vector of junk opts
            std::string oscar = remove_chars(
                make_garbage(rand_me_plz(rand_spec_one, rand_spec_two),
                             rand_me_plz(1, buf_size), "", is_other,
                             never_rand),
                strip_shell);
            if (oscar != "OOR") {
              sep_type = rand_me_plz(0, 1);
              if (sep_type == 0) {
                sys_str = " " + sys_str + *junk_opt + " " + oscar + " ";
              }
              if (sep_type == 1) {
                sys_str =
                    sys_str + " " + *junk_opt + other_sep + oscar + other_sep;
              }
            }
          }
        } else if (rand_buf == false) {
          for (std::vector<std::string>::const_iterator junk_opt_env =
                   junk_opts_env.begin();
               junk_opt_env != junk_opts_env.end();
               ++junk_opt_env) { // loop through the vector of junk envs
            std::string oscar_env = remove_chars(
                make_garbage(rand_me_plz(rand_spec_one, rand_spec_two),
                             buf_size, "", is_other, never_rand),
                " ");
            if (oscar_env != "OOR") {
              env_str = env_str + *junk_opt_env + oscar_env + " ";
            }
          }
          for (std::vector<std::string>::const_iterator junk_opt =
                   junk_opts.begin();
               junk_opt != junk_opts.end();
               ++junk_opt) { // loop through the vector of junk opts
            std::string oscar = remove_chars(
                make_garbage(rand_me_plz(rand_spec_one, rand_spec_two),
                             buf_size, "", is_other, never_rand),
                strip_shell);
            if (oscar != "OOR") {
              sep_type = rand_me_plz(0, 1);
              if (sep_type == 0) {
                sys_str = " " + sys_str + *junk_opt + " " + oscar + " ";
              }
              if (sep_type == 1) {
                sys_str =
                    sys_str + " " + *junk_opt + other_sep + oscar + other_sep;
              }
            }
          }
        }
      }
      std::string out_str;
      std::string out_str_p;
      if (sys_str != "") {
        if (env_str != "") {
          out_str_p = "$(printf \"\\\\x" + binstr_to_hex(env_str) + "\") " +
                    path_str + " $(printf \"\\\\x" + binstr_to_hex(sys_str) +
                    "\") " + always_arg;
        }
        if (env_str == "") {
          out_str_p = path_str + " $(printf \"\\\\x" + binstr_to_hex(sys_str) +
                    "\") " + always_arg;
        }
        out_str = env_str + " " + path_str + " " + sys_str + " " + always_arg;
      }
      out_str = out_str + "; echo $?"; // get the signal
      if (out_str != "; echo $?") {
        junk_opts.clear();
        junk_opts.shrink_to_fit();
        junk_opts_env.clear();
        junk_opts_env.shrink_to_fit();
        if (debug == true) {
          std::ofstream w_f;
          w_f.open(write_file_n, std::ios::out | std::ios::app);
          w_f << out_str << std::endl << out_str_p << std::endl << std::endl;
          w_f.close();
          std::cout << out_str << std::endl << out_str_p << std::endl << std::endl;
        }
        int pid; // initializes child
        FILE *fp =
            popen2(out_str, "r", pid, low_lvl_user); // opens child process fork
        char command_out[4096] = {0};
        std::stringstream output;
        while (read(fileno(fp), command_out, sizeof(command_out) - 1) != 0) {
          output << std::string(command_out);
          memset(&command_out, 0, sizeof(command_out));
        }
        pclose2(fp, pid);
        if (run_command != "") {
          int run_com_pid; // initializes child
          FILE *fp = popen2(run_command, "r", run_com_pid,
                            low_lvl_user); // opens child process fork
          pclose2(fp, run_com_pid);
        }
        std::thread reaper_thread(
            reaper, pid,
            t_timeout); // takes care of killing it off if it takes too long
        reaper_thread.detach();

        std::string token;
        while (std::getline(output, token)) {
          std::regex sf_reg("(132|136|139|135|134|159)"); // regex for the crash
          std::smatch sf;
          if (regex_match(token, sf, sf_reg)) { // match crash
            std::cout << "Crashed with command: " << std::endl
                      << out_str_p << std::endl;
            if (junk_file_of_args != "") {
              std::cout << "File data left in: " << junk_file_of_args
                        << std::endl;
            }
            if (write_to_file == true) {
              write_seg(write_file_n, out_str_p);
              std::cout << "Crash logged." << std::endl;
              exit(0);
            } else {
              exit(0);
            }
          }
        }
      }
    }
  }

  else {
    std::cerr << "Command not found at path..." << std::endl;
    exit(1);
  }
}

int main(int argc, char *argv[]) {
  char *man_chr;
  int opt;
  int num_threads = 2;
  int t_timeout = 3;
  std::vector<std::string> opts;
  std::vector<std::string> spec_env;
  std::vector<std::string> opt_other;
  std::string man_loc = "8";
  std::string buf_size;
  std::string mp;
  std::string template_file;
  std::string strip_shell = "`<>\n|&\[]\()\{}:;";
  std::string u_strip_shell;
  std::string write_file_n = "";
  std::string path_str = "";
  std::string other_sep = "";
  std::string low_lvl_user = "nobody";
  std::string junk_file_of_args;
  std::string always_arg = "";
  std::string run_command = "";
  bool template_opt = false;
  bool man_opt = false;
  bool rand_all = false;
  bool rand_buf = false;
  bool write_to_file = false;
  bool u_strip_shell_set = false;
  bool verbose = false;
  bool debug = false;
  bool is_other = false;
  bool dump_opts = false;
  bool never_rand = false;
  while ((opt = getopt(argc, argv,
                       "m:p:t:e:c:f:o:b:s:x:R:A:F:S:L:W:hrzvdDn")) != -1) {
    switch (opt) {
    case 'v':
      verbose = true;
      break;
    case 'd':
      debug = true;
      break;
    case 't':
      template_opt = true;
      template_file = optarg;
      break;
    case 'c':
      path_str = optarg;
      break;
    case 'b':
      buf_size = optarg;
      break;
    case 'e':
      spec_env = get_flags_template(optarg, verbose, debug);
      break;
    case 'p':
      man_loc = optarg;
      break;
    case 'm':
      man_opt = true;
      man_chr = optarg;
      break;
    case 'f':
      num_threads = std::atoi(optarg);
      break;
    case 'o':
      write_to_file = true;
      write_file_n = optarg;
      break;
    case 'h':
      help_me(argv[0]);
      break;
    case 'r':
      rand_all = true;
      break;
    case 'z':
      rand_buf = true;
      break;
    case 's':
      u_strip_shell = optarg;
      u_strip_shell_set = true;
      break;
    case 'x':
      opt_other = get_other(optarg, verbose, debug);
      is_other = true;
      break;
    case 'D':
      dump_opts = true;
      break;
    case 'S':
      other_sep = optarg;
      break;
    case 'L':
      low_lvl_user = optarg;
      break;
    case 'F':
      junk_file_of_args = optarg;
      break;
    case 'A':
      always_arg = optarg;
      break;
    case 'n':
      never_rand = true;
      break;
    case 'R':
      run_command = optarg;
      break;
    case 'W':
      t_timeout = std::atoi(optarg);
      break;
    default:
      help_me(argv[0]);
    }
  }
  if (u_strip_shell_set == true) {
    strip_shell = u_strip_shell + ">\n";
  }
  if ((man_opt == true) && (template_opt == false)) {
    opts = get_flags_man(man_chr, man_loc, verbose, debug, dump_opts);
  } else if ((man_opt == false) && (template_opt == true)) {
    opts = get_flags_template(template_file, verbose, debug);
  } else if ((man_opt == true) && (template_opt == true)) {
    help_me(argv[0]);
  } else if ((man_opt == false) && (template_opt == false)) {
    help_me(argv[0]);
  } else if (path_str == "") {
    help_me(argv[0]);
  } else {
    help_me(argv[0]);
  }
  std::istringstream b_size(buf_size);
  int is_int;
  if (!(b_size >> is_int)) {
    help_me(argv[0]);
  }
  char buf_char_maybe;
  if (b_size >> buf_char_maybe) {
    help_me(argv[0]);
  } else {
    int buf_size_int = std::stoi(buf_size);
    std::vector<std::thread> threads;
    bool did_it_fault;
    for (int cur_thread = 1; cur_thread <= num_threads; ++cur_thread)
      threads.push_back(std::thread(
          match_seg, buf_size_int, opts, spec_env, path_str, strip_shell,
          rand_all, write_to_file, write_file_n, rand_buf, opt_other, is_other,
          other_sep, t_timeout, low_lvl_user, junk_file_of_args, always_arg,
          never_rand, run_command, verbose, debug)); // Thrift Shop
    for (auto &all_thread : threads)
      all_thread.join(); // is that your grandma's coat?
    exit(0);
  }
}
