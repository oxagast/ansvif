/*
 * ansvif
 * A Not So Very Intelligent Fuzzer
 * Marshall Whittaker / oxagast
 */

//    __ _  _  __   ___  __  ____ ____ 
//   /  ( \/ )/ _\ / __)/ _\/ ___(_  _)
//  (  O )  (/    ( (_ /    \___ \ )(  
//   \__(_/\_\_/\_/\___\_/\_(____/(__)

#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
/*
FILE* popen2_win(std::string command) {
	command = "C:\\Windows\\System32\\WindowsPowerShell\\v1.0\\Powershell.exe " + command;
  FILE *cmd_p;
  cmd_p = popen(command.c_str(), "r");
  if (!cmd_p)
  {
    exit(1);
  }
  return cmd_p;
 }
 
 int pclose2_win(FILE *fp) {
	    pclose(fp);
		return(0);
  }
  */
  
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
			 char   psBuffer[128];
   FILE   *pPipe;

        /* Run DIR so that it writes its output to a pipe. Open this
         * pipe with read text attribute so that we can read it 
         * like a text file. 
         */

   if( (pPipe = _popen( command.c_str(), "rt" )) == NULL )
      return ( pPipe );
std::cout << command << std::endl;
   /* Read pipe until end of file, or an error occurs. */
/*
   while(fgets(psBuffer, 128, pPipe))
   {
      printf(psBuffer);
   }
*/

return (pPipe);
}

/* we have to close it all our so we don't fuck
 * ourselves on OOM later
 */
int pclose2(FILE *fp, pid_t pid) 
{
 /* while ((pid, &stat, 0)) {
    if (errno != EINTR) {
      stat = -1;
      break;
    }
  }
  /* return our status and end pclose2 */
  
     /* Close pipe and print return value of pPipe. */
   if (feof( fp))
   {
   _pclose( fp );
   }
  return -1;
}
