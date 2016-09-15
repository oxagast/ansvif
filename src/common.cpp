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
#include <fstream>
#include <iostream>
#include <regex>
#include <signal.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <thread>
#include <unistd.h>
#include <vector>
#include <signal.h>

#define READ 0
#define WRITE 1

bool match_seg(int buf_size, std::vector<std::string> opts,
               std::vector<std::string> spec_env, std::string path_str,
               std::string strip_shell, bool rand_all, bool write_to_file,
               std::string write_file_n, bool rand_buf,
               std::vector<std::string> opt_other, bool is_other,
               std::string other_sep, int t_timeout, std::string low_lvl_user,
               std::string junk_file_of_args, std::string always_arg_before,
               std::string always_arg_after, bool never_rand,
               std::string run_command, std::string fault_code, bool single_try,
               bool percent_sign, bool verbose, bool debug, std::string ver);
void help_me(std::string mr_me);
std::vector<std::string> get_flags_man(std::string man_page,
                                       std::string man_loc, bool verbose,
                                       bool debug, bool dump_opts);

std::string remove_chars(const std::string &source, const std::string &chars) {
  /* initialize an empty removal string */
  std::string result = "";
  for (unsigned int i = 0; i < source.length(); i++) {
    /* loop through each character removing the ones we
     * don't want
     */
    bool foundany = false;
    for (unsigned int j = 0; j < chars.length() && !foundany; j++) {
      foundany = (source[i] == chars[j]);
    }
    if (!foundany) {
      /* put it back in the buffer if it's not in the list */
      result += source[i];
    }
  }
  return(result);
}

int toint(std::string ints) {
  /* for compatibility with cygwin */
  return atoi(ints.c_str()); 
}

bool file_exists(const std::string &filen) {
  struct stat buf;
  /* return if the file exists or not */
  return (stat(filen.c_str(), &buf) == 0);
}

int rand_me_plz(int rand_from, int rand_to) {
  /* initialize and seed the random device */
  std::random_device rd;
  std::default_random_engine generator(rd());
  /* get a random number from-to */
  std::uniform_int_distribution<int> distribution(rand_from, rand_to);
  /* bind it so we can rerun multiple times for
   * different results
   */
  auto roll = std::bind(distribution, generator);
  return (roll());
}

char fortune_cookie() {
  /* because fortune cookies taste good */
  char chr;
  /* all the characters that make up hex */
  const char *hex_digits = "0123456789ABCDEF";
  int i;
  for (i = 0; i < 1; i++) {
      /* now we just get two random hex characters and
       * return them to the routine
       */
    chr = hex_digits[(rand_me_plz(0, 255))];
  }
  return (chr);
}

int reaper(int grim, int t_timeout) {
  /* run the timer and after the timeout we'll run
   * SIGKILL on it (kill -9 equivilant on linux)
   */
  sleep(t_timeout);
  kill(grim, 9);
  return (0);
}

std::vector<std::string> get_flags_template(std::string filename, bool verbose,
                                            bool debug) {
  /* initialize an options vector and push each option
   * back onto the vector from the template file
   */
  std::vector<std::string> opt_vec;
  std::string line;
  std::ifstream template_file(filename);
  if (template_file.is_open()) {
    while (std::getline(template_file, line)) {
      opt_vec.push_back(line);
    }
    /* we should close it */
    template_file.close();
  } else {
    /* this is incase they supplied a file that wasn't
     * available for some reason
     */
    std::cerr << "Could not open template file..."
              << std::endl;
    exit(1);
  }
  return (opt_vec); // return the vector with the options
}

std::vector<std::string> get_other(std::string filename,
                                   bool verbose, bool debug) {
  /* this is all pretty much the same as above */
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
  /* this is the trash generator, here we generate
   * bad input, some of it random, some of it precrafted
   * and some of it user supplied to try to crash the
   * program at hand
   */
  std::string junk = "";
  std::string hex_stuff;
  int trash_num;
  if (trash == 0) {
    for (trash_num = 0; trash_num < buf; trash_num++) {
      junk = "A" + junk;
    }
  }
  if (trash == 1) {
    junk = "-1";
  }
  if (trash == 2) {
    junk = "1";
  }
  if (trash == 3) {
    junk = "0";
  }
  if (trash == 4) {
    junk = "2";
  }
  if (trash == 5) {
    int hex_null_i = 0x00;
    std::stringstream hex_null_ss;
    hex_null_ss << hex_null_i;
    junk = hex_null_ss.str();
  }
  if (trash == 6) {
    for (trash_num = 0; trash_num < buf / 2; trash_num++) {
      junk = "\%s" + junk;
    }
  }
  if (trash == 7) {
    for (trash_num = 0; trash_num < buf / 2; trash_num++) {
      junk = "\%n" + junk;
    }
  }
  if (never_rand == false) {
    if (trash == 8) {
      for (trash_num = 0; trash_num < buf; trash_num++) {
        junk = junk += fortune_cookie();
      }
    }
  }
  if (trash == 9) { // front
    for (trash_num = 0; trash_num < buf; trash_num++) {
      junk = "A" + junk;
    }
    junk = user_junk + junk;
    if (buf - user_junk.length() < junk.size())
      junk = junk.substr(0, buf);
    else
      return ("OOR");
  }
  if (trash == 10) {
    for (trash_num = 0; trash_num < buf / 2; trash_num++) {
      junk = "\%s" + junk;
    }
    junk = user_junk + junk;
    if (buf - user_junk.length() < junk.size())
      junk = junk.substr(0, buf);
    else
      return ("OOR");
  }
  if (never_rand == false) {
    if (trash == 11) {
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
  if (trash == 12) {
    for (trash_num = 0; trash_num < buf; trash_num++) {
      junk = "A" + junk;
    }
    junk = junk + user_junk;
    if (buf - user_junk.length() < junk.size())
      junk = junk.substr(junk.length() - buf);
    else
      return ("OOR");
  }
  if (trash == 13) {
    for (trash_num = 0; trash_num < buf / 2; trash_num++) {
      junk = "\%s" + junk;
    }
    junk = junk + user_junk;
    if (buf - user_junk.length() < junk.size())
      junk = junk.substr(junk.length() - buf);
    else
      return ("OOR");
  }
  if (never_rand == false) {
    if (trash == 14) {
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
  if (trash == 15) {
    for (trash_num = 0; trash_num < buf / 2; trash_num++) {
      junk = "\%n" + junk;
    }
    junk = junk + user_junk;
    if (buf - user_junk.length() < junk.size())
      junk = junk.substr(junk.length() - buf);
    else
      return ("OOR");
  }
  if (trash == 16) {
    junk = opt_other_str;
  }
  /* return the junk to put in between the args */
  return (junk);
}

std::string make_garbage(int trash, int buf, std::string opt_other_str,
                         bool is_other, bool never_rand) {
  buf = buf - 1;
  std::string all_junk;
  if (is_other == true) {
    if (isatty(STDIN_FILENO)) {
      /* if it's a stdin then we'll call the tash generator */
      std::string user_stuff = "";
      all_junk = trash_generator(trash, buf, user_stuff, opt_other_str,
                                 never_rand);
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
  /* return all the junk the trash generator made */
  return (all_junk);
}

void write_seg(std::string filename, std::string line) {
  /* this is just a simple file writing routine
   * used mostly for logging and writing the junk
   * files
   */
  std::ofstream w_f;
  w_f.open(filename, std::ios::out | std::ios::app);
  w_f << line << std::endl;
  w_f.close();
}

void write_junk_file(std::string filename, std::vector<std::string> opt_other,
                     int buf_size, int rand_spec_one, int rand_spec_two,
                     bool never_rand, std::string other_sep, bool verbose) {
  /* if there is an old file we should remove it first */
  remove(filename.c_str());
  /* initialize our junk and write oscar to the file */
  std::string oscar;
  std::ofstream w_f;
  w_f.open(filename, std::ios::out | std::ios::app);
  for (int start_buf = 0; start_buf <= buf_size; start_buf++) {
    std::string oscar = opt_other.at(
        rand_me_plz(0, opt_other.size() - 1));
    std::string trash = make_garbage(rand_me_plz(rand_spec_one, rand_spec_two),
                                     rand_me_plz(1, buf_size), "", false,
                                     never_rand);
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
        /* write the shit to terminal if we're in verbose mode */
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
  /* make sure to close it out */
  w_f.close();
}
