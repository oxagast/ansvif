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
#include <sys/wait.h>
#include <thread>
#include <unistd.h>

#define READ 0
#define WRITE 1

FILE *popen2(std::string command, std::string type, int &pid,
             std::string low_lvl_user) {
  pid_t child_pid;
  int fd[2];
  pid = pipe(fd);
  if ((child_pid = fork()) == -1) {
    perror("fork");
    exit(1);
  }
  /* here the child begins */
  if (child_pid == 0) {
    if (type == "r") {
      /* redirect stdout and stdin to pipe */
      close(fd[READ]);
      dup2(fd[WRITE], 1);
    } else {
      close(fd[WRITE]);
      dup2(fd[READ], 0);
    }
    if (getuid() == 0) {
      /* if we're root we're going to drop our privs 
       * this fixes not being able to reap processes that
       * are suid
       */
      execl("/bin/su", "su", "-c", "/bin/sh", "-c", command.c_str(),
            low_lvl_user.c_str(),
            NULL);
    } else {
      /* or just run it like we normally would */
      execl("/bin/sh", "/bin/sh", "-c", command.c_str(), NULL);
    }
    exit(0);
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
}

/* we have to close it all our so we don't fuck
 * ourselves on OOM later
 */
int pclose2(FILE *fp, pid_t pid) 
{
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
}
