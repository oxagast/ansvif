#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <iostream>
#include <string>
#include <sstream>
#include <dirent.h>
#include <vector>

bool is_suid(const char *file) {
  struct stat results;
  stat(file, &results);
  if (results.st_mode & S_ISUID) return true;
  return false;
}

void help_me(char *me) {
  std::cout << "Usage:" << std::endl << " " << me
            << " /bin/ /usr/sbin/ /usr/bin/ /usr/bin/libexec/" << std::endl;
  exit(1);
}

int main(int argc, char **argv) {
  if (argc < 2) help_me(argv[0]);
  std::string file_str;
  std::vector<std::string> file_list;
  for (int path_num = 1; path_num != argc; path_num++) {
    const char *path = argv[path_num];
    DIR *the_dir;
    struct dirent *this_dir;
    the_dir = opendir(path);
    if (the_dir != NULL)
      while ((this_dir = readdir(the_dir)))
        file_list.push_back(std::string(this_dir->d_name));
    std::string name;
    int file_list_size = file_list.size();
    for (int file_num = 0; file_num != file_list_size; file_num++) {
      name = file_list[file_num];
      std::string path_to_file = std::string(path) + file_list[file_num];
      if (is_suid(path_to_file.c_str()) == true)
        std::cout << path_to_file << std::endl;
    }
    file_list.clear();
  }
  exit(0);
}
