#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int ret_code = 1;

  if (argc >= 3) {
    if (strcmp(argv[1], "fchownat") == 0) {
      printf("%s\n", argv[1]);
      /* int fchownat(int dirfd, const char *pathname, uid_t owner, gid_t group,
       * int flags); */
      ret_code = fchownat(argv[2], argv[3], argv[4], argv[5], argv[6]);
    }
    if (strcmp(argv[1], "chmod") == 0) {
      /* int chmod(const char *pathname, mode_t mode); */
      printf("%s\n", argv[1]);
      ret_code = chmod(argv[2], argv[3]);
    }
  } else {
    printf("Not enough arguments.\n");
  }
  return (ret_code);
}
