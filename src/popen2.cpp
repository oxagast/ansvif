/*
 * ansvif
 * A Not So Very Intelligent Fuzzer
 * Marshall Whittaker / oxagast
 */

//    __ _  _  __   ___  __  ____ ____
//   /  ( \/ )/ _\ / __)/ _\/ ___(_  _)
//  (  O )  (/    ( (_ /    \___ \ )(
//   \__(_/\_\_/\_/\___\_/\_(____/(__)

#include <iomanip>
#include <string>
#include <sys/stat.h>
#ifdef __unix__
#include <sys/wait.h>
#endif
#include <thread>
#include <unistd.h>
#ifdef __FREEBSD
#include <cstdio>
#include <errno.h>
#include <stdlib.h>
#endif

#define READ 0
#define WRITE 1

FILE *popen2(std::string command, std::string type, int &pid,
             std::string low_lvl_user) {
#ifdef __unix__
  /* here the child begins */
 pid_t child_pid;
  int fd[2];
  pid = pipe(fd);
  if ((child_pid = fork()) == -1) {
    perror("fork");
    exit(1);
  }
  /* here the child begins */
  if (child_pid == 0) {

#ifdef __NOTANDROID__
    if (getuid() == 0) {
      /* if we're root we're going to drop our privs
       * this fixes not being able to reap processes that
       * are suid
       */
      // command = "DISPLAY=localhost:1 " + command;
      execl("/bin/su", "su", "-c", "/bin/sh", "-c", command.c_str(),
         low_lvl_user.c_str(), NULL);
    } else {
      /* or just run it like we normally would */
      execl("/bin/sh", "sh", "-c", command.c_str(), NULL);
    }
#endif
#ifdef __ANDROID__
      //command = "'" + command + "'";
      execl("/system/bin/sh", "sh", "-c", command.c_str(), NULL);
#endif

  } else {
    if (type == "r") {
      close(fd[WRITE]);
    } else {
      close(fd[READ]);
    }
  }
  /* our new process should now equal the child's pid */
  pid = child_pid;
  if (type == "r") {
    /* return the junk to the rest of the program */
    return fdopen(fd[READ], "r");
}
  return fdopen(fd[WRITE], "w");
#endif
#ifdef _WIN32
  //command =
  //          command;
  /* char ps_buffer[128]; */
  FILE *process_pipe;
  /* Run DIR so that it writes its output to a pipe. Open this
   * pipe with read text attribute so that we can read it
   * like a text file.
   */
  if ((process_pipe = _popen(command.c_str(), "rt")) == NULL)
    return (process_pipe);
  /* std::cout << command << std::endl; */
  return (process_pipe);
#endif
}



/* we have to close it all our so we don't fuck
 * ourselves on OOM later
 */
int pclose2(FILE *fp, pid_t pid) {
#ifdef __unix__
  int stat;
  fclose(fp);
  while (waitpid(pid, &stat, 0) == 0) {
    if (errno != EINTR) {
      stat = -1;
      break;
    }
  }
  /* return our status and end pclose2 */
  return stat;
#endif
#ifdef _WIN32
  /* we have to close it all our so we don't fuck
   * ourselves on OOM later
   */
  /* return our status and end pclose2 */
  if (feof(fp)) {
    _pclose(fp);
  }
  return -1;
#endif
}

