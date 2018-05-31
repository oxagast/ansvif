/*
 * ansvif
 * A Not So Very Intelligent Fuzzer
 * Marshall Whittaker / oxagast
 */

//    __ _  _  __   ___  __  ____ ____
//   /  ( \/ )/ _\ / __)/ _\/ ___(_  _)
//  (  O )  (/    ( (_ /    \___ \ )(
//   \__(_/\_\_/\_/\___\_/\_(____/(__)

#ifdef __unix
#include "src/version.h"
#endif
#ifdef _WIN32
#include "version.h"
#endif
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <string>

void help_me(std::string mr_me) {
  /* print all the help shit to STDOUT */
  std::cout
      << "ansvif v" << ver << " -- A Not So Very Intelligent Fuzzer"
      << std::endl
      << "Usage:" << std::endl
      << " " << mr_me << " -t template -c ./faulty -b 2048" << std::endl
      << "Options:" << std::endl
      << " -t [file]    This file should hold line by line command arguments."
      << std::endl
      << "              as shown in the example file." << std::endl
      << " -e [file]    This file should hold line by line environment "
         "variables."
      << std::endl
      << "              as shown in the example file.  You can" << std::endl
      << "              usually get these by doing something like:" << std::endl
      << "              $ strings /bin/mount | perl -ne 'print if /[A-Z]=$/' > "
         "mount_envs"
      << std::endl
      << " -c [path]    Specifies the command path." << std::endl
#ifdef __linux
      << " -p [integer] Specifies the manpage location (as an integer, usually "
         "1 or 8)."
      << std::endl
      << " -m [command] Specifies the commands manpage." << std::endl
#endif
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
      << "              characters are: <>\\n |&\[]\()\{}:;\\ and mandatory "
         "omitted"
      << std::endl
      << "              characters are: >\\n."
      << std::endl
      << " -o [file]    Log to this file." << std::endl
      << " -x [file]    Other opts to put in, such as usernames, etc."
      << std::endl
      << " -S \",\"       Some seperator besides 'space' between opts, such as "
         "',:-' etc."
      << std::endl
      << " -L           Privilege changing not implimented in Windows yet.  "
         "Defaults"
      << std::endl
      << "              nobody." << std::endl
      << " -A \"foo\"     Always put this string after the command."
      << std::endl
      << " -B \"bar\"     Always put this string before the command."
      << std::endl
      << " -F [file]    A file with junk to be fuzzed with whole." << std::endl
      << " -n           Never use random data in the fuzz." << std::endl
      << " -C \"13\"      Non default crash recognition error codes."
      << std::endl
#ifdef _WIN32
      << "              Defaults are -1073741819, -1073740791, -1073741571, and"
      << std::endl
      << "              -532459699."
#endif
#ifdef __linux
      << "              Defaults are 132, 136, 139, 135, 134, and 159."
#endif
      << std::endl
      << " -W [integer] Thread timeout." << std::endl
#ifdef _WIN32
      << " -V           Valgrind is not supported under Windows." << std::endl
#endif
#ifdef __linux
      << " -V           Fuzz in conjunction with Valgrind." << std::endl
#endif
      << " -1           Try to induce a crash once, and if a crash is not "
         "induced,"
      << std::endl
      << "              throw error code 64." << std::endl
      << " -P           Use % to represent binary in fuzz." << std::endl
      << " -M           Max arguments to use in the fuzz." << std::endl
      << " -y           Short for -b 0 and usually only useful with -A or -B"
      << std::endl
      << " -K           Keep going after a crash." << std::endl
      << " -E \"cmd\"     A command to be run before the fuzzed program." << std::endl
      << " -R \"ls\"      Always run this command after the fuzz." << std::endl
      << " -N \"prog\"    Shorthand for -R \"pkill prog\"" << std::endl
      << " -0           No NULL in fuzz." << std::endl
      << " -v           Be verbose." << std::endl
      << " -d           Debug data." << std::endl
      << " -i           Prints version information." << std::endl;
  /* exit with an error because we didn't do anything but
   * print a help page
   */
  exit(1);
}
