/*
 * ansvif
 * A Not So Very Intelligent Fuzzer
 * Marshall Whittaker / oxagast
 */

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
  if (child_pid == 0) { // child begins
    if (type == "r") {
      close(fd[READ]);    // Close the READ
      dup2(fd[WRITE], 1); // Redirect stdout to pipe
    } else {
      close(fd[WRITE]);  // Close the WRITE
      dup2(fd[READ], 0); // Redirect stdin to pipe
    }
    if (getuid() == 0) { // if we're root...
      execl("/bin/su", "su", "-c", "/bin/sh", "-c", command.c_str(),
            low_lvl_user.c_str(),
            NULL); // fixes not being able to reap suid 0 processes
    } else {
      execl("/bin/sh", "/bin/sh", "-c", command.c_str(),
            NULL); // runs it all if we're not using a lower level user (or
                   // root itself)
    }
    exit(0); // exit cleanly, wtf
  } else {
    if (type == "r") {  // if we're reading
      close(fd[WRITE]); // Close the WRITE
    } else {            // or...
      close(fd[READ]);  // Close the READ
    }
  }
  pid = child_pid;   // our process id shall now equal the child's pid
  if (type == "r") { // if we're reading...
    return fdopen(fd[READ], "r"); // return the stuff to the rest of the program
  }
  return fdopen(fd[WRITE],
                "w"); // return the stuff to the rest of the program too
} // the end of popen2

int pclose2(FILE *fp, pid_t pid) // close it so we don't fuck outselves
{
  int stat;   // initialize stat
  fclose(fp); // close the pipe
  while (waitpid(pid, &stat, 0) ==
         0) {             // while we're waiting for pid and stat to be 0...
    if (errno != EINTR) { // if we errored out...
      stat = -1;          // stat will now be -1 so we know we did
      break;              // break out of the while
    }
  }
  return stat; // return our status
} // the end of pclose2
