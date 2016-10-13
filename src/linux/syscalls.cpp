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
#include <sys/epoll.h>
#include <sys/eventfd.h>

int calls(std::string caller, std::string arg1, std::string arg2,
          std::string arg3, std::string arg4, std::string arg5) {
  std::cout << caller << std::endl;
  int ret = 255;
  if (caller == "accept") {
    /*  ret_code = accept(int sockfd, struct sockaddr *addr, socklen_t
     * *addrlen); 
     */
    int accept(int sockfd, struct sockaddr *arg1, socklen_t *arg2);
  }
  else if (caller == "accept4") {
    /* ret_code = accept4(int sockfd, struct sockaddr *addr, socklen_t *addrlen,
     * int flags); 
     */
    int accept4(int sockfd, struct sockaddr *arg1, socklen_t *arg2, int flags);
  }
  else if (caller == "access") {
    /* int access(const char *pathname, int mode); */
   ret = access(arg1.c_str(), atoi(arg2.c_str()));
  }
  else if (caller == "acct") {
  /* int acct(const char *filename); */
    ret = acct(arg1.c_str());
  }
  else if (caller == "alarm") {
    /* unsigned int alarm(int seconds); */
    alarm(atol(arg1.c_str()));
  }
  else if (caller == "bind") {
    /* int bind(int sockfd, const struct sockaddr *addr,
     *           socklen_t addrlen);
     */
    int bind(int sockfd, const struct sockaddr *arg1, socklen_t *arg2);
  }
  else if (caller == "bpf") {
    /* int bpf(int cmd, union bpf_attr *attr, unsigned int size); */
    int bpf(int cmd, union bpf_attr *arg1, unsigned int size);
  }
  else if (caller == "capget") {
    /* int capget(cap_user_header_t hdrp, cap_user_data_t datap); */
    int capget(cap_user_header_t *arg1, cap_user_data_t *arg2);
  }
  else if (caller == "capset") {
    /* int capset(cap_user_header_t hdrp, const cap_user_data_t datap); */
    int capset(cap_user_header_t *arg1, const cap_user_data_t *arg2);
  }
  else if (caller == "chdir") {
    /* int chdir(const char *path); */
    ret = chdir(arg1.c_str());
  }
  else if (caller == "chmod") {
    /* int chmod(const char *pathname, mode_t mode); */
    ret = chmod(arg1.c_str(), atoi(arg2.c_str()));
  }
  else if (caller == "chroot") {
    /* int chroot(const char *path); */
    ret = chroot(arg1.c_str());
  }
  else if (caller == "clock_getres") {
    /* int clock_getres(clockid_t clk_id, struct timespec *res); */
    int clock_getres(clockid_t *arg1, struct timespec *arg2);
  }
  else if (caller == "clock_gettime") {
    /* int clock_gettime(clockid_t clk_id, struct timespec *tp); */
    int clock_gettime(clockid_t *arg1, struct timespec *arg2);
  }
  else if (caller == "clock_nanosleep") {
    /* int clock_nanosleep(clockid_t clock_id, int flags,
     *                      const struct timespec *request,
     *                      struct timespec *remain);
     */
    int clock_nanosleep(clockid_t *arg1, int flags,
                           const struct timespec *arg2,
                           struct timespec *arg3);
  }
  else if (caller == "clock_settime") {
    /* int clock_settime(clockid_t clk_id, const struct timespec *tp); */
    int clock_settime(clockid_t arg1, const struct timespec *arg2);
  }
  else if (caller == "clone") {
    /* long clone(unsigned long flags, void *child_stack,
     *            void *ptid, void *ctid,
     *            struct pt_regs *regs);
     */
    long clone(unsigned long flags, void *child_stack,
                 void *ptid, void *ctid,
                 struct pt_regs *arg1);
  }
  else if (caller == "connect") {
    /* int connect(int sockfd, const struct sockaddr *addr,
     *              socklen_t addrlen);
     */
    int connect(int sockfd, const struct sockaddr *arg1,
                   socklen_t arg2);
  }
  else if (caller == "creat") {
    /* int creat(const char *pathname, mode_t mode); */
    ret = creat(arg1.c_str(), atoi(arg2.c_str()));
  }
  else if (caller == "dup") {
    /* int dup(int oldfd) */
    ret = dup(atoi(arg1.c_str()));
  }
  else if (caller == "dup2") {
    /* int dup2(int oldfd, int newfd) */
    ret = dup2(atoi(arg1.c_str()), atoi(arg2.c_str()));
  }
  else if (caller == "dup3") {
    /* int dup3(int oldfd, int newfd, int flags); */
    ret = dup3(atoi(arg1.c_str()), atoi(arg2.c_str()), atoi(arg3.c_str()));
  }
  else if (caller == "epoll_create") {
    /* int epoll_create(int size); */
    ret = epoll_create(atoi(arg1.c_str()));
  }
  else if (caller == "epoll_create1") {
    /* int epoll_create1(int flags); */
    ret = epoll_create1(atoi(arg1.c_str()));
  }
  else if (caller == "epoll_ctl") {
    /* int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event); */
    int epoll_ctl(int epfd, int op, struct epoll_event *arg1);
  }
  else if (caller == "epoll_pwait") {
    /* int epoll_pwait(int epfd, struct epoll_event *events,
     *                 int maxevents, int timeout,
     *                 const sigset_t *sigmask);
     */
    int epoll_pwait(int epfd, struct epoll_event *arg1,
                      int maxevents, int timeout,
                      const sigset_t *arg2);
  }
  else if (caller == "epoll_wait") {
    /* int epoll_wait(int epfd, struct epoll_event *events,
     *                 int maxevents, int timeout);
     */
    int epoll_wait(int epfd, struct epoll_event *arg1,
                      int maxevents, int timeout);
  }
  else if (caller == "eventfd") {
    /* int eventfd(unsigned int initval, int flags); */
    int eventfd(unsigned int initval, int flags);
  }
  else if (caller == "faccessat") {
    /* int faccessat(int dirfd, const char *pathname, int mode, int flags); */
    ret = faccessat(atoi(arg1.c_str()), arg2.c_str(), atoi(arg3.c_str()),
                    atoi(arg4.c_str()));
  }
  else if (caller == "fallocate") {
    /* int fallocate(int fd, int mode, off_t offset, off_t len); */
    int fallocate(int fd, int mode, off_t *arg1, off_t *arg2);
  }
  else if (caller == "posix_fadvise") {
    /* int posix_fadvise(int fd, off_t offset, off_t len, int advice); */
    int posix_fadvise(int fd, off_t *arg1, off_t *arg2, int advice);
  }
  else if (caller == "fchownat") {
    /* int fchownat(int dirfd, const char *pathname, uid_t owner, gid_t group,
     * int flags); 
     */
    ret = fchownat(atoi(arg1.c_str()), arg2.c_str(), atoi(arg3.c_str()),
                   atoi(arg4.c_str()), atoi(arg5.c_str()));
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