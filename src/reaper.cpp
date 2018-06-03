#include <string>
#include <signal.h>
#include <cstdlib>
#include <unistd.h>

int reaper(int grim, int t_timeout, std::string just_kill_me) {
#ifdef __unix__
   /* run the timer and after the timeout we'll run
   * SIGKILL on it (kill -9 equivilant on linux)
   */
  //std::cout << " pid: " << grim << " timeout " << t_timeout << std::endl;
  sleep(t_timeout);
  kill(grim, 15);
  /* if the above doesn't work, we can fall back to piggybacking pkill
   * if they use -N
   */
  if ( just_kill_me != "") {
    just_kill_me = "pkill " + just_kill_me;
    system(just_kill_me.c_str());
  }
  return (0);
#elif _WIN32
/* windows doesn't support kill 9 */
#else
  return (0);
#endif
}

