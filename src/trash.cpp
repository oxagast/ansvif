#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <thread>
#include <unistd.h>
#include <vector>

int rand_me_plz(int rand_from, int rand_to);

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
    std::stringstream int_trash;
    int_trash << rand_me_plz(0,9999);
    junk = int_trash.str();
    }
  if (trash == 17) {
         std::stringstream int_trash, float_trash;
    int_trash << rand_me_plz(0,9999);
    float_trash << rand_me_plz(0,9999);
     junk = int_trash.str() + "." + float_trash.str();
  }
  if (trash == 18) {
      junk = "\"\"";
  }
  if (trash == 19) {
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
