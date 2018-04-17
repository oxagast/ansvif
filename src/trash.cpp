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

switch(trash) {
  case 0x00 :
{
    for (trash_num = 0; trash_num < buf; trash_num++) {
      junk = "A" + junk;
    }
      break;
}
  case 0x01 :
{
     junk = "-1";
     break;
}
  case 0x02 :
{
     junk = "1";
     break;
}
  case 0x03 :
{
     junk = "0";
     break;
}
  case 0x040 :
{
     junk = "2";
     break;
}
  case 0x05 : 
{
     std::stringstream hex_null_ss;
     hex_null_ss << 0x00;
     junk = hex_null_ss.str();
     break;
}
  case 0x06 :
{
     for (trash_num = 0; trash_num < buf / 2; trash_num++) {
       junk = "\%s" + junk;
     }
     break;
}
  case 0x07 :
{
     for (trash_num = 0; trash_num < buf / 2; trash_num++) {
       junk = "\%n" + junk;
     }
break;
}
  case 0x08 :
{
     if (never_rand == false) {
       for (trash_num = 0; trash_num < buf; trash_num++) {
         junk = junk += fortune_cookie();
       }
     }
break;
}
   case 0x09 :
{
    for (trash_num = 0; trash_num < buf; trash_num++) {
      junk = "A" + junk;
    }
    junk = user_junk + junk;
    if (buf - user_junk.length() < junk.size())
      junk = junk.substr(0, buf);
    else {
      return ("OOR");
}
break;
}
case 0x0A :
{
    for (trash_num = 0; trash_num < buf / 2; trash_num++) {
      junk = "\%s" + junk;
    }
    junk = user_junk + junk;
    if (buf - user_junk.length() < junk.size())
      junk = junk.substr(0, buf);
    else
      return ("OOR");
break;
}
case 0x0B :
{
  if (never_rand == false) {
      for (trash_num = 0; trash_num < buf; trash_num++) {
        junk = junk += fortune_cookie();
      }
      junk = user_junk + junk;
      if (buf - user_junk.length() < junk.size())
        junk = junk.substr(0, buf);
      else
        return ("OOR");
}
break;
}
case 0x0C :
{
    for (trash_num = 0; trash_num < buf; trash_num++) {
      junk = "A" + junk;
    }
    junk = junk + user_junk;
    if (buf - user_junk.length() < junk.size())
      junk = junk.substr(junk.length() - buf);
    else
      return ("OOR");
break;
}
case 0x0D :
{
    for (trash_num = 0; trash_num < buf / 2; trash_num++) {
      junk = "\%s" + junk;
    }
    junk = junk + user_junk;
    if (buf - user_junk.length() < junk.size())
      junk = junk.substr(junk.length() - buf);
    else
      return ("OOR");
break;
}
case 0x0E :
{
  if (never_rand == false) {
      for (trash_num = 0; trash_num < buf; trash_num++) {
        junk = junk += fortune_cookie();
      }
      junk = junk + user_junk;
      if (buf - user_junk.length() < junk.size())
        junk = junk.substr(junk.length() - buf);
      else
        return ("OOR");
    }
break;
}
case 0x0F :
{
    for (trash_num = 0; trash_num < buf / 2; trash_num++) {
      junk = "\%n" + junk;
    }
    junk = junk + user_junk;
    if (buf - user_junk.length() < junk.size())
      junk = junk.substr(junk.length() - buf);
    else
      return ("OOR");
break;
}
case 0x10 :
{
    std::stringstream int_trash;
    int_trash << rand_me_plz(0, 9999);
    junk = int_trash.str();
break;
}
case 0x11 :
{
    std::stringstream int_trash, float_trash;
    int_trash << rand_me_plz(0, 9999);
    float_trash << rand_me_plz(0, 9999);
    junk = int_trash.str() + "." + float_trash.str();
break;
}
case 0x12 :
{
    junk = "\"\"";
}
break;
case 0x13 :
{
    junk = opt_other_str;
break;
}
case 0x14 :
{
    junk = "-32768";
break;
}
case 0x15 :
{
    junk = "32767";
break;
}
case 0x16 :
{
    junk = "-2147483648";
break;
}
case 0x17 :
{
    junk = "2147483647";
break;
}
case 0x18 :
{
    junk = "-9223372036854775808";
break;
}
case 0x19 :
{
    junk = "9223372036854775807";
break;
}
case 0x1A :
{
    junk = "65535";
break;
}
case 0x1B :
{
    junk = "65536";
break;
}
case 0x1C :
{
    junk = "4294967295";
break;
}
case 0x1D :
{
    junk = "18446744073709551615";
break;
}
case 0x1E :
{
    junk = "18446744073709551616";
break;
}
case 0x1F :
{
    junk = "127";
break;
}
case 0x20 :
{
    junk = "255";
break;
}
case 0x21 :
{
    junk = "256";
    break;
}
default :
return ("OOR");
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
  /* return all the junk the trash generator made */
  return ("'" + all_junk + "'");
}
