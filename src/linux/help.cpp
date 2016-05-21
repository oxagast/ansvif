/*
 * ansvif
 * A Not So Very Intelligent Fuzzer
 * Marshall Whittaker / oxagast
 */

#include <stdio.h>
#include <string>
#include <iostream>
#include <cstdlib>

void help_me(std::string mr_me, std::string ver) {
  std::cout
      << "ansvif v" << ver << " -- A Not So Very Intelligent Fuzzer" << std::endl
      << "Usage:" << std::endl << " " << mr_me
      << " -t template -c ./faulty -b 2048" << std::endl
      << "Options:" << std::endl
      << " -t [file]    This file should hold line by line command arguments"
      << std::endl << "              as shown in the example file." << std::endl
      << " -e [file]    This file should hold line by line environment "
         "variables" << std::endl
      << "              as shown in the example file.  You can" << std::endl
      << "              usually get these by doing something like:" << std::endl
      << "              $ strings /bin/mount | perl -ne 'print if /[A-Z]=$/' > "
         "mount_envs" << std::endl
      << " -c [path]    Specifies the command path." << std::endl
      << " -p [integer] Specifies the manpage location (as an integer, usually "
         "1 or 8)." << std::endl
      << " -m [command] Specifies the commands manpage." << std::endl
      << " -D           Dump what we can out of the manpage to stdout."
      << std::endl << " -f [integer] Number of threads to use.  Default is 2."
      << std::endl << " -b [integer] Specifies the buffer size to fuzz with."
      << std::endl << "              256-2048 Is usually sufficient."
      << std::endl << " -r           Use only random data." << std::endl
      << " -z           Randomize buffer size from 1 to specified by the -b "
         "option." << std::endl
      << " -s \"@#^$CE\"  Characters to omit from randomization.  Default "
         "omitted" << std::endl
      << "              characters are: <>\\n |&\[]\()\{}:;\\ and mandatory "
         "omitted" << std::endl << "              characters are: >\\n"
      << std::endl << " -o [file]    Log to this file." << std::endl
      << " -x [file]    Other opts to put in, such as usernames, etc."
      << std::endl
      << " -S \",\"       Some seperator besides 'space' between opts, such as "
         "',:-' etc." << std::endl
      << " -L [nobody]  An unprivileged user to run as if you're root.  "
         "Defaults nobody." << std::endl
      << " -A \"foo\"     Always put this string after the command."
      << std::endl
      << " -B \"bar\"     Always put this string before the command."
      << std::endl << " -F [file]    A file with junk to be fuzzed with whole."
      << std::endl << "              Must be used with an -x template."
      << std::endl << " -n           Never use random data in the fuzz."
      << std::endl << " -R \"ls\"      Always run this command after the fuzz."
      << std::endl
      << " -C \"13\"      Non default crash recognition error codes."
      << std::endl
      << "              Defaults are 132, 136, 139, 135, 134, and 159."
      << std::endl << " -W [integer] Thread timeout." << std::endl
      << " -V           Use Valgrind if installed." << std::endl
      << " -1           Try to induce a crash once, and if a crash is not "
         "induced," << std::endl << "              throw error code 64."
      << std::endl << " -P           Use % to represent binary in fuzz."
      << std::endl << " -v           Verbose." << std::endl
      << " -d           Debug."
      << std::endl;  // write all this help file shit out to STDOUT
  exit(1);  // exit with error 1 because we didn't really do anything but print
            // help...
}
