#include <cstdlib>
#include <fcntl.h>
#include <iostream>
#include <random>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int calls(std::string caller, std::string arg1, std::string arg2,
          std::string arg3, std::string arg4, std::string arg5) {
  int ret = 1;
  std::cout << caller << std::endl;
  if (caller == "accept") {
    /*  ret_code = accept(int sockfd, struct sockaddr *addr, socklen_t
     * *addrlen); */
    int accept(int sockfd, struct sockaddr *addr, socklen_t arg1);
  }
  if (caller == "accept4") {

    /* ret_code = accept4(int sockfd, struct sockaddr *addr, socklen_t *addrlen,
     * int flags); */
    int accept4(int sockfd, struct sockaddr *addr, socklen_t arg1, int flags);
  }
  if (caller == "access") {

    /* int access(const char *pathname, int mode); */
    access(arg1.c_str(), atoi(arg2.c_str()));
  }
  if (caller == "faccessat") {

    /* int faccessat(int dirfd, const char *pathname, int mode, int flags); */
    ret = faccessat(atoi(arg1.c_str()), arg2.c_str(), atoi(arg3.c_str()),
                    atoi(arg4.c_str()));
  }
  if (caller == "fchownat") {

    /* int fchownat(int dirfd, const char *pathname, uid_t owner, gid_t group,
     * int flags); */
    ret = fchownat(atoi(arg1.c_str()), arg2.c_str(), atoi(arg3.c_str()),
                   atoi(arg4.c_str()), atoi(arg5.c_str()));
  }
  if (caller == "chmod") {
    /* int chmod(const char *pathname, mode_t mode); */

    ret = chmod(arg1.c_str(), atoi(arg2.c_str()));
  }
  return (ret);
}

int main(int argc, char *argv[]) {
  std::string arg1, arg2, arg3, arg4, arg5;
  if (argv[1] == NULL) {
    exit(1);
  }
  if (argv[2] == NULL) {
    arg1 = "";
  } else {
    arg1 = std::string(argv[2]);
  }
  if (argv[3] == NULL) {
    arg2 = "";
  } else {
    arg2 = std::string(argv[3]);
  }
  if (argv[4] == NULL) {
    arg3 = "";
  } else {
    arg3 = std::string(argv[4]);
  }
  if (argv[5] == NULL) {
    arg4 = "";
  } else {
    arg4 = std::string(argv[5]);
  }
  if (argv[6] == NULL) {
    arg5 = "";
  } else {
    arg5 = std::string(argv[6]);
  }
  if (argc >= 2) {
    int ret_code = calls(std::string(argv[1]), arg1, arg2, arg3, arg4, arg5);
    return (ret_code);

  } else {
    std::cout << "Not enough arguments to fuzz." << std::endl;
  }
}