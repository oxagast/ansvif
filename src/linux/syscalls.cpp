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
#include <sys/fanotify.h>
#include <linux/bpf.h>
#include <asm/ptrace.h>

struct epoll_event event;
struct timespec res_ts, tp_ts, rq_ts, rm_ts, st_ts;
sigset_t sigmask;
struct sockaddr acc, socka;
socklen_t addrlen;
union bpf_attr *attr;
cap_user_header_t cuh;
cap_user_data_t cud;

int calls(std::string caller, std::string arg1, std::string arg2,
          std::string arg3, std::string arg4, std::string arg5) {
  std::cout << caller << std::endl;
  int ret = 0;
  if (caller == "accept") {
    /*  ret_code = accept(int sockfd, struct sockaddr *addr, socklen_t
     * *addrlen); 
     */
    ret = accept(atoi(arg1.c_str()), &acc, &addrlen);
  }
  else if (caller == "accept4") {
    /* ret_code = accept4(int sockfd, struct sockaddr *addr, socklen_t *addrlen,
     * int flags); 
     */
    ret = accept4(atoi(arg1.c_str()), &acc, &addrlen, atoi(arg2.c_str()));
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
    ret = bind(atoi(arg1.c_str()), &socka, addrlen);
  }
  else if (caller == "bpf") {
    /* int bpf(int cmd, union bpf_attr *attr, unsigned int size); */
    /* ret = bpf(atoi(arg1.c_str()), &attr, atoi(arg2.c_str())); */
  }
  else if (caller == "capget") {
    /* int capget(cap_user_header_t hdrp, cap_user_data_t datap); */
    /* ret = capget(cuh, cud); */
  }
  else if (caller == "capset") {
    /* int capset(cap_user_header_t hdrp, const cap_user_data_t datap); */
    /* int capset(cap_user_header_t *arg1, const cap_user_data_t *arg2); */
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
    ret = clock_getres(atoi(arg1.c_str()), &res_ts);
  }
  else if (caller == "clock_gettime") {
    /* int clock_gettime(clockid_t clk_id, struct timespec *tp); */
    ret = clock_gettime(atoi(arg1.c_str()), &tp_ts);
  }
  else if (caller == "clock_nanosleep") {
    /* int clock_nanosleep(clockid_t clock_id, int flags,
     *                      const struct timespec *request,
     *                      struct timespec *remain);
     */
    ret = clock_nanosleep(atoi(arg1.c_str()), atoi(arg2.c_str()), &rq_ts, &rm_ts);
  }
  else if (caller == "clock_settime") {
    /* int clock_settime(clockid_t clk_id, const struct timespec *tp); */
    ret = clock_settime(atoi(arg1.c_str()), &st_ts);
  }
  else if (caller == "clone") {
    /* long clone(unsigned long flags, void *child_stack,
     *            void *ptid, void *ctid,
     *            struct pt_regs *regs);
     */
  }
  else if (caller == "connect") {
    /* int connect(int sockfd, const struct sockaddr *addr,
     *              socklen_t addrlen);
     */
    ret = connect(atoi(arg1.c_str()), &socka,
                   addrlen);
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
    ret = epoll_ctl(atoi(arg1.c_str()), atoi(arg2.c_str()), atoi(arg3.c_str()), &event);
  }
  else if (caller == "epoll_pwait") {
    /* int epoll_pwait(int epfd, struct epoll_event *events,
     *                 int maxevents, int timeout,
     *                 const sigset_t *sigmask);
     */
    ret = epoll_pwait(atoi(arg1.c_str()), &event,
                      atoi(arg2.c_str()), atoi(arg3.c_str()),
                      &sigmask);
  }
  else if (caller == "epoll_wait") {
    /* int epoll_wait(int epfd, struct epoll_event *events,
     *                 int maxevents, int timeout);
     */
    ret = epoll_wait(atoi(arg1.c_str()), &event,
                      atoi(arg2.c_str()), atoi(arg3.c_str()));
  }
  else if (caller == "eventfd") {
    /* int eventfd(unsigned int initval, int flags); */
    ret = eventfd(atoi(arg1.c_str()), atoi(arg1.c_str()));
  }
  else if (caller == "faccessat") {
    /* int faccessat(int dirfd, const char *pathname, int mode, int flags); */
    ret = faccessat(atoi(arg1.c_str()), arg2.c_str(), atoi(arg3.c_str()),
                    atoi(arg4.c_str()));
  }
  else if (caller == "fallocate") {
    /* int fallocate(int fd, int mode, off_t offset, off_t len); */
      ret = fallocate(atoi(arg1.c_str()), atoi(arg2.c_str()), atoi(arg3.c_str()), atoi(arg4.c_str()));
  }
  else if (caller == "posix_fadvise") {
    /* int posix_fadvise(int fd, off_t offset, off_t len, int advice); */
    ret = posix_fadvise(atoi(arg1.c_str()), atoi(arg2.c_str()), atoi(arg3.c_str()), atoi(arg4.c_str()));
  }
  else if (caller == "fchownat") {
    /* int fchownat(int dirfd, const char *pathname, uid_t owner, gid_t group,
     * int flags); 
     */
    ret = fchownat(atoi(arg1.c_str()), arg2.c_str(), atoi(arg3.c_str()),
                   atoi(arg4.c_str()), atoi(arg5.c_str()));
  }
  else if (caller == "fanotify_init") {
      ret = fanotify_init(atoi(arg1.c_str()), atoi(arg2.c_str()));
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