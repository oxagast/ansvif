/*
 * ansvif
 * A Not So Very Intelligent Fuzzer
 * Marshall Whittaker / oxagast
 */

//    __ _  _  __   ___  __  ____ ____ 
//   /  ( \/ )/ _\ / __)/ _\/ ___(_  _)
//  (  O )  (/    ( (_ /    \___ \ )(  
//   \__(_/\_\_/\_/\___\_/\_(____/(__)

#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

FILE* popen2_win(std::string command) {
	command = "powershell -c " + command;
  FILE *cmd_p = popen(command.c_str(), "r");
  if (!cmd_p)
  {
    exit(1);
  }
  return cmd_p;
 }
 
 int pclose2_win(FILE *fp) {
	    pclose(fp);
		return(0);
  }