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
#include <string.h>
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
#include <sys/file.h>
#include <sys/vfs.h>
#include <sys/time.h>

/* #include <numaif.h> */

#include <syscall.h>
#include <linux/futex.h>
#include <linux/unistd.h>

/* #include <asm/ldt.h> */

/*
#include <linux/getcpu.h>
#include <include/asm/linkage.h>
#include <linux/stringify.h>
#include <generated/timeconst.h>
#include <asm/unistd_64_x32.h>
*/

/*
#include "linux/module.h"
#include "linux/list.h"
#include <asm/linkage.h>
#include <generated/timeconst.h>
problems with including the linux kernel headers
*/


struct epoll_event event;
struct timespec res_ts, tp_ts, rq_ts, rm_ts, st_ts;
sigset_t sigmask;
struct sockaddr acc, socka;
socklen_t addrlen;
union bpf_attr *attr;
cap_user_header_t cuh;
cap_user_data_t cud;
struct stat statbuff;
struct statfs statfsbuff;
off_t ftruncate_len;
struct timeval times[2];
struct robust_list_head **head_ptr;
size_t len_ptr;
//struct user_desc u_info;
unsigned node;
//struct getcpu_cache tcache;
 size_t cwd_size;
 struct linux_dirent *dirp;
 
long calls(std::string caller, std::string arg1, std::string arg2,
          std::string arg3, std::string arg4, std::string arg5) {
  std::cout << caller << std::endl;
  long ret = 0;
  if (caller == "accept") {
    /*  ret_code = accept(int sockfd, struct sockaddr *addr, socklen_t
     * *addrlen); 
     */
    ret = accept(atoi(arg1.c_str()), &acc, &addrlen);
    ret = accept(atoi(arg1.c_str()), NULL, &addrlen);
    ret = accept(atoi(arg1.c_str()), &acc, NULL);
    ret = accept(atoi(arg1.c_str()), NULL, NULL);
  }
  else if (caller == "accept4") {
    /* ret_code = accept4(int sockfd, struct sockaddr *addr, socklen_t *addrlen,
     * int flags); 
     */
    ret = accept4(atoi(arg1.c_str()), &acc, &addrlen, atoi(arg2.c_str()));
    ret = accept4(atoi(arg1.c_str()), NULL, &addrlen, atoi(arg2.c_str()));
    ret = accept4(atoi(arg1.c_str()), &acc, NULL, atoi(arg2.c_str()));
    ret = accept4(atoi(arg1.c_str()), NULL, NULL, atoi(arg2.c_str()));
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
    ret = bind(atoi(arg1.c_str()), NULL, addrlen);
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
    ret = clock_getres(atoi(arg1.c_str()), NULL);
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
    ret = clock_nanosleep(atoi(arg1.c_str()), atoi(arg2.c_str()), NULL, &rm_ts);
    ret = clock_nanosleep(atoi(arg1.c_str()), atoi(arg2.c_str()), &rq_ts, NULL);
    ret = clock_nanosleep(atoi(arg1.c_str()), atoi(arg2.c_str()), NULL, NULL);
  }
  else if (caller == "clock_settime") {
    /* int clock_settime(clockid_t clk_id, const struct timespec *tp); */
    ret = clock_settime(atoi(arg1.c_str()), &st_ts);
    ret = clock_settime(atoi(arg1.c_str()), NULL);
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
    ret = connect(atoi(arg1.c_str()), &socka, addrlen);
    ret = connect(atoi(arg1.c_str()), NULL, addrlen);
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
    ret = epoll_ctl(atoi(arg1.c_str()), atoi(arg2.c_str()), atoi(arg3.c_str()), NULL);
  }
  else if (caller == "epoll_pwait") {
    /* int epoll_pwait(int epfd, struct epoll_event *events,
     *                 int maxevents, int timeout,
     *                 const sigset_t *sigmask);
     */
    ret = epoll_pwait(atoi(arg1.c_str()), &event,
                      atoi(arg2.c_str()), atoi(arg3.c_str()),
                      &sigmask);
    ret = epoll_pwait(atoi(arg1.c_str()), NULL,
                      atoi(arg2.c_str()), atoi(arg3.c_str()),
                      &sigmask);
    ret = epoll_pwait(atoi(arg1.c_str()), &event,
                      atoi(arg2.c_str()), atoi(arg3.c_str()),
                      NULL);
    ret = epoll_pwait(atoi(arg1.c_str()), NULL,
                      atoi(arg2.c_str()), atoi(arg3.c_str()),
                      NULL);
  }
  else if (caller == "epoll_wait") {
    /* int epoll_wait(int epfd, struct epoll_event *events,
     *                 int maxevents, int timeout);
     */
    ret = epoll_wait(atoi(arg1.c_str()), &event,
                      atoi(arg2.c_str()), atoi(arg3.c_str()));
    ret = epoll_wait(atoi(arg1.c_str()), NULL,
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
  else if (caller == "fanotify_init") {
    /* int fanotify_init(unsigned int flags, unsigned int event_f_flags); */
     ret = fanotify_init(atoi(arg1.c_str()), atoi(arg2.c_str()));
  }
  else if (caller == "fchdir") {
    /* int fchdir(int fd); */
    ret = fchdir(atoi(arg1.c_str()));
  }
  else if (caller == "fchmod") {
    /* int fchmod(int fd, mode_t mode); */
    ret = fchmod(atoi(arg1.c_str()),atoi(arg2.c_str()));
  }
  else if (caller == "fchmodat") {
    /* int fchmodat(int dirfd, const char *pathname, mode_t mode, int flags); */
    ret = fchmodat(atoi(arg1.c_str()), arg2.c_str(), atoi(arg3.c_str()), atoi(arg4.c_str()));
  }
  else if (caller == "fchown") {
    /* int fchown(int fd, uid_t owner, gid_t group); */
    ret = fchown(atoi(arg1.c_str()), atoi(arg2.c_str()), atoi(arg3.c_str()));
  }
  else if (caller == "fchownat") {
    /* int fchownat(int dirfd, const char *pathname, uid_t owner, gid_t group,
     * int flags); 
     */
    ret = fchownat(atoi(arg1.c_str()), arg2.c_str(), atoi(arg3.c_str()),
                   atoi(arg4.c_str()), atoi(arg5.c_str()));
  }
  else if (caller == "fdatasync") {
    /* int fdatasync(int fd); */
    ret = fdatasync(atoi(arg1.c_str()));
  }
  else if (caller == "finit_module") {
    /* int finit_module(int fd, const char *param_values,
     *                   int flags);
     */
/*
    ret = finit_module(atoi(arg1.c_str()), arg2.c_str(), atoi(arg3.c_str())); // problems with this on kernel headers
*/
  }
  else if (caller == "flock") {
    /* int flock(int fd, int operation); */
    ret = flock(atoi(arg1.c_str()), atoi(arg2.c_str()));
  }
  else if (caller == "fstat") {
    /* int fstat(int fd, struct stat *buf); */
    ret = fstat(atoi(arg1.c_str()), &statbuff);
    ret = fstat(atoi(arg1.c_str()), NULL);
  }
  else if (caller == "fstatat") {
    /* int fstatat(int dirfd, const char *pathname, struct stat *buf,
     *                  int flags);
     */
    ret = fstatat(atoi(arg1.c_str()), arg2.c_str(), &statbuff, atoi(arg3.c_str()));
    ret = fstatat(atoi(arg1.c_str()), arg2.c_str(), NULL, atoi(arg3.c_str()));
  }
  else if (caller == "fstatfs") {
    /* int fstatfs(int fd, struct statfs *buf); */
    ret = fstatfs(atoi(arg1.c_str()), &statfsbuff);
  }
  else if (caller == "fsync") {
    /* int fsync(int fd); */
    ret = fsync(atoi(arg1.c_str()));
  }
  else if (caller == "ftruncate") {
    /* int ftruncate(int fd, off_t length); */
    ret = ftruncate(atoi(arg1.c_str()), ftruncate_len);
  }
  else if (caller == "futimesat") {
    /* int futimesat(int dirfd, const char *pathname,
     *                const struct timeval times[2]);
     */
    futimesat(atoi(arg1.c_str()), arg2.c_str(), &times[2]);
    futimesat(atoi(arg1.c_str()), arg2.c_str(), NULL);
  }
  else if (caller == "get_mempolicy") {
    /* int get_mempolicy(int *mode, unsigned long *nodemask,
     *                    unsigned long maxnode, unsigned long addr,
     *                    unsigned long flags);
     */
  /*
    ret = get_mempolicy(atoi(arg1.c_str()), atoul(arg2.c_str()),
                         atoul(arg3.c_str()), atoul(arg4.c_str()),
                         atoul(arg5.c_str()));
  */
  }
  else if (caller == "get_robust_list") {
    /* long get_robust_list(int pid, struct robust_list_head **head_ptr,
     *                       size_t *len_ptr);
     */
/*
    ret = get_robust_list(atoi(arg1.c_str()), &head_ptr,
                            &len_ptr);
*/
  }
  else if (caller == "get_thread_area") {
    /* int get_thread_area(struct user_desc *u_info); */
    /* ret = get_thread_area(&u_info); */
  }
  else if (caller == "getcpu") {
    /* int getcpu(unsigned *cpu, unsigned *node, struct getcpu_cache *tcache); */
    /* ret = getcpu(atoi(arg1.c_str()), &node, &tcache); */
  }
  else if (caller == "getcwd") {
    /* char *getcwd(char *buf, size_t size); */
   // char *arg1char;
    char *arg1char = const_cast<char *>(arg1.c_str());
    getcwd(arg1char, cwd_size);
  }
  else if (caller == "getdents") {
    /* int getdents(unsigned int fd, struct linux_dirent *dirp,
     *               unsigned int count);
     */
    /* ret = getdents(atoi(arg1.c_str()), &dirp, atoi(arg2.c_str())); */
  }
  else if (caller == "getegid") {
    /* gid_t getegid(void); */
   gid_t getegid(void);
  }
  else if (caller == "getgid") {
    gid_t getgid(void);
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
  if ((argv[2] == NULL) || (strcmp(argv[2], "\"\"") == 0)) {
    arg1.assign(nullptr, 0);
  } else {
    arg1 = std::string(argv[2]);
  }
  if ((argv[3] == NULL) || (strcmp(argv[3], "\"\"") == 0)) {
    arg2.assign(nullptr, 0);
  } else {
    arg2 = std::string(argv[3]);
  }
  if ((argv[4] == NULL) || (strcmp(argv[4], "\"\"") == 0)) {
    arg3.assign(nullptr, 0);
  } else {
    arg3 = std::string(argv[4]);
  }
  if ((argv[5] == NULL) || (strcmp(argv[5], "\"\"") == 0)) {
    arg4.assign(nullptr, 0);
  } else {
    arg4 = std::string(argv[5]);
  }
  if ((argv[6] == NULL) || (strcmp(argv[6], "\"\"") == 0)) {
    arg5.assign(nullptr, 0);
  } else {
    arg5 = std::string(argv[6]);
  }
  if (argc >= 2) {
    long ret_code = calls(std::string(argv[1]), arg1, arg2, arg3, arg4, arg5);
    return (ret_code);

  } else {
    std::cout << "Not enough arguments to fuzz." << std::endl;
  }
}