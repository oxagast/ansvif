/*
 * ansvif
 * A Not So Very Intelligent Fuzzer
 * Marshall Whittaker / oxagast
 */

#include <dirent.h>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <vector>

bool is_suid(const char *file) {
  struct stat results;  // build the holder
  stat(file, &results); // find our file and stat it
  if (results.st_mode & S_ISUID)
    return true; // if it's suid return true
  return false;  // else false
}

void help_me(char *me) {
  std::cout << "Usage:" << std::endl
            << " " << me << " /bin/ /usr/sbin/ /usr/bin/ /usr/bin/libexec/"
            << std::endl; // some useage details
  exit(1);                // error out because we didn't have proper input
}

int main(int argc, char **argv) {
  if (argc < 2)
    help_me(argv[0]);   // if there is less than one argument, then go to the
                        // help page
  std::string file_str; // initialize the string for the file
  std::vector<std::string> file_list; // initialize the file list vector
  for (int path_num = 1; path_num != argc;
       path_num++) { // for each file in the list...
    //    const char *path = argv[path_num];  // do for  the current path
    // char *path2;
    std::string check_path = argv[path_num];
    std::string path_str;
    if (check_path.substr(check_path.length() - 1) !=
        "/") { // so that we have '/'
      path_str = argv[path_num];
      path_str = path_str + "/"; // '/'
    } else {
      path_str = argv[path_num]; // otherwise just that
    }
    DIR *the_dir;                        // the dir to use
    struct dirent *this_dir;             // we're using dirent.h for 'this_dir'
    the_dir = opendir(path_str.c_str()); // open the path to check the files
    if (the_dir != NULL)                 // if it's not nothing...
      while ((this_dir = readdir(the_dir))) // and while we're reading...
        file_list.push_back(std::string(
            this_dir->d_name)); // put it in the file list if it's suid 0
    std::string name;           // initialize the name
    int file_list_size =
        file_list.size(); // so that we don't get a warning with -Wall about
                          // unsigned variables being compared to integars
    for (int file_num = 0; file_num != file_list_size;
         file_num++) {            // we'll loop through them
      name = file_list[file_num]; // put the file name into the 'name' variable
                                  // for the time being
      std::string path_to_file =
          //          std::string(path) +
          //          file_list[file_num];  // put it all together (the path and
          //          filename)
          path_str + file_list[file_num];
      if (is_suid(path_to_file.c_str()) == true)
        std::cout << path_to_file
                  << std::endl; // if it's suid 0 then we'll print it to STDOUT
    }
    file_list.clear(); // clear out the file list vector
  }
  exit(0); // success
}
