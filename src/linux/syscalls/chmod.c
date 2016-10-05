#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>           /* Definition of AT_* constants */

int main (int argc, char *argv[]) {
/*  int chmod(const char *pathname, mode_t mode); */
chmod(argv[1], argv[2]);
return (0);
}
