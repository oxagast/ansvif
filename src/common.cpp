/*
 * ansvif
 * A Not So Very Intelligent Fuzzer
 * Marshall Whittaker / oxagast
 */

#include <thread>
#include <sys/wait.h>
#include <sys/stat.h>
#include <vector>
#include <unistd.h>
#include <cstdlib>
#include <regex>
#include <iostream>
#include <stdio.h>
#include <fstream>
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
  std::string result = "";  // initialize an empty removal
  for (unsigned int i = 0; i < source.length();
       i++) {  // start at being through end
    bool foundany = false;
    for (unsigned int j = 0; j < chars.length() && !foundany; j++) {
      foundany = (source[i] == chars[j]);  // get the chars
    }
    if (!foundany) {
      result += source[i];  // only put it back if it's not in the list
    }
  }
  return (result);  // return the command
}

int toint(std::string ints) {
  return atoi(ints.c_str());  // for compatability with cygwin and mingw
}

bool file_exists(const std::string &filen) {
  struct stat buf;  // get it
  return (stat(filen.c_str(), &buf) ==
          0);  //  return the status of if its a file or not
}

int rand_me_plz(int rand_from, int rand_to) {
  std::random_device rd;
  std::default_random_engine generator(rd());  // seed random
  std::uniform_int_distribution<int> distribution(rand_from,
                                                  rand_to);  // get a random
  auto roll = std::bind(distribution,
                        generator);  // bind it so we can do it multiple times
  return (roll());
}

char fortune_cookie() {  // because they're good
  char chr;
  const char *hex_digits = "0123456789ABCDEF";  // all the hex characters
  int i;
  for (i = 0; i < 1;
       i++) {  // get two random hex characters (single parts of one)
    chr = hex_digits[(rand_me_plz(0, 255))];  // get a random one
  }
  return (chr);  // return it
}

int reaper(int grim, int t_timeout) {  // is coming to get you...
  sleep(t_timeout);                    // after the timeout...
  kill(grim, 9);                       // run a SIGKILL on it...
  return (0);                          // truth, we kilt it
}

std::vector<std::string> get_flags_template(std::string filename, bool verbose,
                                            bool debug) {
  std::vector<std::string> opt_vec;       // initialize the options vector
  std::string line;                       // a line out of the file initializer
  std::ifstream template_file(filename);  // grab it from the filename
  if (template_file.is_open()) {          // if it opened...
    while (std::getline(template_file, line)) {  // for each line
      opt_vec.push_back(line);  // put the options in the vector list
    }
    template_file.close();  // close the file
  } else {
    std::cerr << "Could not open template file..."
              << std::endl;  // sorry we couldn't find the template?
    exit(1);                 // error out
  }
  return (opt_vec);  // return the vector with the options
}

std::vector<std::string> get_other(
    std::string filename,
    bool verbose,  // this is pretty much the same as above
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
  if (trash == 0) {  // kosher
    for (trash_num = 0; trash_num < buf; trash_num++) {
      junk = "A" + junk;  // put lots of As
    }
  }
  if (trash == 1) {
    for (trash_num = 0; trash_num < buf; trash_num++) {
      junk = "9" + junk;  //  lots of 9s
    }
  }
  if (trash == 2) {
    for (trash_num = 0; trash_num < buf / 2; trash_num++) {
      junk = "\%s" + junk;  // for string format vulns
    }
  }
  if (never_rand == false) {
    if (trash == 3) {
      for (trash_num = 0; trash_num < buf; trash_num++) {
        junk = junk += fortune_cookie();  // a n y t h i n g ! ! !
      }
    }
  }
  if (trash == 4) {  // front
    for (trash_num = 0; trash_num < buf; trash_num++) {
      junk = "A" + junk;  // put lots of As
    }
    junk = user_junk + junk;
    if (buf - user_junk.length() < junk.size())
      junk = junk.substr(0, buf);
    else
      return ("OOR");
  }
  if (trash == 5) {
    for (trash_num = 0; trash_num < buf; trash_num++) {
      junk = "9" + junk;  // yadda yadda
    }
    junk = user_junk + junk;
    if (buf - user_junk.length() < junk.size())
      junk = junk.substr(0, buf);
    else
      return ("OOR");
  }
  if (trash == 6) {
    for (trash_num = 0; trash_num < buf / 2; trash_num++) {
      junk = "\%s" + junk;  // yadda yadda
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
    for (trash_num = 0; trash_num < buf; trash_num++) {  // back
      junk = "A" + junk;                                 // put lots of As
    }
    junk = junk + user_junk;
    if (buf - user_junk.length() < junk.size())
      junk = junk.substr(junk.length() - buf);
    else
      return ("OOR");
  }
  if (trash == 9) {
    for (trash_num = 0; trash_num < buf; trash_num++) {
      junk = "9" + junk;  // yadda yadda
    }
    junk = junk + user_junk;
    if (buf - user_junk.length() < junk.size())
      junk = junk.substr(junk.length() - buf);
    else
      return ("OOR");
  }
  if (trash == 10) {
    for (trash_num = 0; trash_num < buf / 2; trash_num++) {
      junk = "\%s" + junk;  // yadda yadda
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
    junk = opt_other_str;  // if its the other stuff from stdin put that
  }
  return (junk);  // return the junk to put between the args
}

std::string make_garbage(int trash, int buf, std::string opt_other_str,
                         bool is_other, bool never_rand) {
  buf = buf - 1;
  std::string all_junk;
  if (is_other == true) {
    if (isatty(STDIN_FILENO)) {  // if its a stdin
      std::string user_stuff = "";
      all_junk = trash_generator(trash, buf, user_stuff, opt_other_str,
                                 never_rand);  // call the trash generator
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
  return (all_junk);  // return the 'junk' that the trash generator made
}

void write_seg(std::string filename, std::string line) {
  std::ofstream w_f;                                  // call it w_f
  w_f.open(filename, std::ios::out | std::ios::app);  // open it for writing
  w_f << line << std::endl;                       // write the fault
  w_f.close();                                        // close it
}

void write_junk_file(std::string filename, std::vector<std::string> opt_other,
                     int buf_size, int rand_spec_one, int rand_spec_two,
                     bool never_rand, std::string other_sep, bool verbose) {
  remove(filename.c_str());  // remove it if there is an old file
  std::string oscar;         // initialize our junk
  std::ofstream w_f;         // call it w_f
  w_f.open(filename, std::ios::out | std::ios::app);
  for (int start_buf = 0; start_buf <= buf_size; start_buf++) {
    std::string oscar = opt_other.at(
        rand_me_plz(0, opt_other.size() - 1));  // put stuff in our oscar junk
    std::string trash = make_garbage(rand_me_plz(rand_spec_one, rand_spec_two),
                                     rand_me_plz(1, buf_size), "", false,
                                     never_rand);  // make some racket
    w_f << oscar;                                  // write it!
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
        if (trash != "OOR") std::cerr << trash;
      }
      if (is_sep == 1) {
        std::cerr << other_sep << std::endl;
      } else {
        std::cerr << std::endl;
      }
    }
  }
  w_f.close();  // close it
}
