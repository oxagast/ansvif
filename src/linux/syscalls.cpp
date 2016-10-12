/*
 * ansvif
 * A Not So Very Intelligent Fuzzer
 * Marshall Whittaker / oxagast
 */

//    __ _  _  __   ___  __  ____ ____ 
//   /  ( \/ )/ _\ / __)/ _\/ ___(_  _)
//  (  O )  (/    ( (_ /    \___ \ )(  
//   \__(_/\_\_/\_/\___\_/\_(____/(__)

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
#include <linux/capability.h>

int calls(std::string caller, std::string arg1, std::string arg2,
          std::string arg3, std::string arg4, std::string arg5) {
  std::cout << caller << std::endl;
  int ret = -1;
  if (caller == "alarm") {
    alarm(atol(arg1.c_str()));
  }
  else if (caller == "bind") {
    /* int bind(int sockfd, const struct sockaddr *addr,
     *           socklen_t addrlen);
     */
    int bind(int sockfd, const struct sockaddr arg1, socklen_t arg2);
  }
  else if (caller == "bpf") {
    /* int bpf(int cmd, union bpf_attr *attr, unsigned int size); */
    int bpf(int cmd, union bpf_attr arg1, unsigned int size);
  }
  else if (caller == "capget") {
    /* int capget(cap_user_header_t hdrp, cap_user_data_t datap); */
    int capget(cap_user_header_t arg1, cap_user_data_t arg2);
  }
  else if (caller == "capset") {
    /* int capset(cap_user_header_t hdrp, const cap_user_data_t datap); */
    int capset(cap_user_header_t arg1, const cap_user_data_t arg2);
  }
  else if (caller == "chdir") {
    /* int chdir(const char *path); */
    ret = chdir(arg1.c_str());
  }
  else if (caller == "chroot") {
    /* int chroot(const char *path); */
    ret = chroot(arg1.c_str());
  }
  else if (caller == "clock_getres") {
    /* int clock_getres(clockid_t clk_id, struct timespec *res); */
    int clock_getres(clockid_t arg1, struct timespec arg2);
  }
  else if (caller == "clock_gettime") {
    /* int clock_gettime(clockid_t clk_id, struct timespec *tp); */
    int clock_gettime(clockid_t arg1, struct timespec arg2);
  }
  else if (caller == "clock_settime") {
    /* int clock_settime(clockid_t clk_id, const struct timespec *tp); */
    int clock_settime(clockid_t arg1, const struct timespec arg2);
  }
  else if (caller == "clock_nanosleep") {
    /* int clock_nanosleep(clockid_t clock_id, int flags,
     *                      const struct timespec *request,
     *                      struct timespec *remain);
     */
    int clock_nanosleep(clockid_t arg1, int flags,
                           const struct timespec arg2,
                           struct timespec arg3);
  }
  else if (caller == "clone") {
    /* long clone(unsigned long flags, void *child_stack,
     *            void *ptid, void *ctid,
     *            struct pt_regs *regs);
     */
    long clone(unsigned long flags, void *child_stack,
                 void *ptid, void *ctid,
                 struct pt_regs arg1);
  }
  else if (caller == "connect") {
    /* int connect(int sockfd, const struct sockaddr *addr,
     *              socklen_t addrlen);
     */
    int connect(int sockfd, const struct sockaddr arg1,
                   socklen_t arg2);
  }
  else if (caller == "creat") {
    /* int creat(const char *pathname, mode_t mode); */
    ret = creat(arg1.c_str(), atoi(arg2.c_str()));
  }
  else if (caller == "accept") {
    /*  ret_code = accept(int sockfd, struct sockaddr *addr, socklen_t
     * *addrlen); 
     */
    int accept(int sockfd, struct sockaddr arg1, socklen_t arg2);
  }
  else if (caller == "accept4") {
    /* ret_code = accept4(int sockfd, struct sockaddr *addr, socklen_t *addrlen,
     * int flags); 
     */
    int accept4(int sockfd, struct sockaddr arg1, socklen_t arg2, int flags);
}
  else if (caller == "access") {
    /* int access(const char *pathname, int mode); */
   ret = access(arg1.c_str(), atoi(arg2.c_str()));
  }
  else if (caller == "acct") {
  /* int acct(const char *filename); */
    ret = acct(arg1.c_str());
  }
  else if (caller == "faccessat") {
    /* int faccessat(int dirfd, const char *pathname, int mode, int flags); */
    ret = faccessat(atoi(arg1.c_str()), arg2.c_str(), atoi(arg3.c_str()),
                    atoi(arg4.c_str()));
  }
  else if (caller == "fchownat") {
    /* int fchownat(int dirfd, const char *pathname, uid_t owner, gid_t group,
     * int flags); 
     */
    ret = fchownat(atoi(arg1.c_str()), arg2.c_str(), atoi(arg3.c_str()),
                   atoi(arg4.c_str()), atoi(arg5.c_str()));
  }
  else if (caller == "chmod") {
    /* int chmod(const char *pathname, mode_t mode); */
    ret = chmod(arg1.c_str(), atoi(arg2.c_str()));
  }
  else {
    std::cout << "Syscall not found..." << std::endl;
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