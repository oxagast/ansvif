#include <fcntl.h>           /* Definition of AT_* constants */
#include <unistd.h>

int main (int argc, char *argv[]) {
/* int fchownat(int dirfd, const char *pathname, uid_t owner, gid_t group, int flags); */
fchownat(argv[1], argv[2], argv[3], argv[4], argv[5]);
return (0);
}
