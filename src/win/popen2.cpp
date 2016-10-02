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
#include <thread>
#include <unistd.h>

#define READ 0
#define WRITE 1

FILE *popen2(std::string command, std::string type, int &pid,
             std::string low_lvl_user) {
  command = "cmd /c C:\\Windows\\SysWOW64\\WindowsPowerShell\\v1.0\\Powershell.exe -v 5 -c " + command;
   /* char ps_buffer[128]; */
   FILE *process_pipe;
  /* Run DIR so that it writes its output to a pipe. Open this
   * pipe with read text attribute so that we can read it 
   * like a text file. 
   */
   if( (process_pipe = _popen( command.c_str(), "rt" )) == NULL )
      return ( process_pipe );
   /* std::cout << command << std::endl; */
  return (process_pipe);
}

/* we have to close it all our so we don't fuck
 * ourselves on OOM later
 */
int pclose2(FILE *fp, pid_t pid) 
{
  /* return our status and end pclose2 */
   if (feof( fp))
   {
   _pclose( fp );
   }
  return -1;
}
