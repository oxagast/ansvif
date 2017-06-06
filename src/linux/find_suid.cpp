/*
 * ansvif
 * A Not So Very Intelligent Fuzzer
 * Marshall Whittaker / oxagast
 */

//    __ _  _  __   ___  __  ____ ____
//   /  ( \/ )/ _\ / __)/ _\/ ___(_  _)
//  (  O )  (/    ( (_ /    \___ \ )(
//   \__(_/\_\_/\_/\___\_/\_(____/(__)

#include <dirent.h>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <vector>

bool is_suid(const char *file) {
  /* build the holder, find our  file and stat it */
  struct stat results;
  stat(file, &results);
  /* check if the file is suid and if it is return
   * true
   */
  if (results.st_mode & S_ISUID) {
    return true;
  }
  /* if it's not then we'll return false */
  else {
    return false;
  }
}

void help_me(char *me) {
  /* we're gonna print a little help page if they
   * didn't put in correct input
   */
  std::cout << "Usage:" << std::endl
            << " " << me << " /bin/ /usr/sbin/ /usr/bin/ /usr/bin/libexec/"
            << std::endl;
  exit(1);
}

int main(int argc, char **argv) {
  if (argc < 2)
    /* if there is less than one argument than go
     * to the help page
     */
    help_me(argv[0]);
  /* initialize our varaibles like the string for the file
   * and the list of files vector
   */
  std::string file_str;
  std::vector<std::string> file_list;
  /* for each file in the list  we'll have the check path
   * and the path string
   */
  for (int path_num = 1; path_num != argc; path_num++) {
    std::string check_path = argv[path_num];
    std::string path_str;
    if (check_path.substr(check_path.length() - 1) != "/") {
      /* make sure that we have a '/' at the end */
      path_str = argv[path_num];
      path_str = path_str + "/";
    }
    /* otherwise we'll just use what they put if they
     * already had the slash at the end of the path
     */
    else {
      path_str = argv[path_num];
    }
    /* we declare the pointer to the dir to use, and use dirent.h
     * for this_dir and open the path to check for files, make sure
     * that the path isn't empty
     */
    DIR *the_dir;
    struct dirent *this_dir;
    the_dir = opendir(path_str.c_str());
    /* if the dir exists then we'll read the dir and put the file
     * in the list if it is suid
     */
    if (the_dir != NULL) {
      while ((this_dir = readdir(the_dir))) {
        file_list.push_back(std::string(this_dir->d_name));
      }
      /* initialize the name variable for a filename placeholder */
      std::string name;
      /* we do this so that with warnings on we don't spew a bunch of
       * unsigned variables being compared to integars warnings
       */
      int file_list_size = file_list.size();
      /* we'll now loop through the files and put the file name into
       * the name variable placeholder
       */
      for (int file_num = 0; file_num != file_list_size; file_num++) {
        name = file_list[file_num];
        std::string path_to_file = path_str + file_list[file_num];
        if (is_suid(path_to_file.c_str()) == true) {
          /* if the file is suid then we will cough it up to STDOUT */
          std::cout << path_to_file << std::endl;
        }
      }
    }
    /* clear out the file list vector */
    file_list.clear();
  }
  /* we're done, success! */
  exit(0);
}
