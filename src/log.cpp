#include "../include/xmlwriter/xml_writer.hpp"
#ifdef __unix
#include "src/version.h"
#endif
#ifdef _WIN32
#include "version.h"
#endif
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <unistd.h>

void log_head(std::string write_file_n, std::string path_str,
              std::string cmd_output, std::string out_str_p,
              std::string out_str, int pid) {
/* all this xml stuff is for logging */
#ifdef __unix__
  std::ostringstream pid_as_s;
  pid_as_s << pid;
  std::string output_logfile_pid =
      write_file_n + ".output." + pid_as_s.str().c_str() + ".ansvif.log";
  std::string crash_logfile_pid =
      write_file_n + ".crash." + pid_as_s.str().c_str() + ".ansvif.log";

#endif
  std::ofstream xml_output;
  xml_output.open(write_file_n + ".crash.ansvif.log");
  Writer writer(xml_output);
  writer.openElt("ansvif");
  writer.openElt("Version")
      .attr("ver", "The ansvif version to fuzzing with")
      .content(ver.c_str())
      .closeElt();
  writer.openElt("Program")
      .attr("path", "Path of the file fuzzed")
      .content(path_str.c_str())
      .closeElt();
#ifdef __unix__
  writer.openElt("Process")
      .attr("PID", "The process ID of the crashed program")
      .content(pid_as_s.str().c_str())
      .closeElt();
  ;
#endif
}

void log_tail(std::string write_file_n, std::string junk_file_of_args,
              std::string output_logfile, std::string crash_logfile,
              std::string cmd_output, std::string out_str_p,
              std::string out_str, int pid) {
/* since we crashed we're going to finish writing to the
*xml file
*/
#ifdef __unix__
  std::ostringstream pid_as_s;
  pid_as_s << pid;
  std::string output_logfile_pid =
      write_file_n + ".output." + pid_as_s.str().c_str() + ".ansvif.log";
  std::string crash_logfile_pid =
      write_file_n + ".crash." + pid_as_s.str().c_str() + ".ansvif.log";
#endif
  std::ofstream xml_output;
  xml_output.open(write_file_n + ".crash.ansvif.log");
  Writer writer(xml_output);
  writer.openElt("Crash");
  writer.openElt("Exit Code")
      .attr("code", "The programs exit code")
      .content(cmd_output.c_str())
      .closeElt();
  writer.openElt("Command")
      .attr("run", "What the command crashed with")
      .content(out_str_p.c_str())
      .closeElt();
  writer.openElt("Command")
      .attr("run_plain", "What the command crashed with (plaintext)")
      .content(out_str.c_str())
      .closeElt();
  writer.openElt("File data")
      .attr("file", "File data used left here")
      .content(junk_file_of_args.c_str())
      .closeAll();
  xml_output.close();
  std::cout << "Crash logged." << std::endl;
/* move the logged files for pid */
#ifdef __unix__
  rename(output_logfile.c_str(), output_logfile_pid.c_str());
  rename(crash_logfile.c_str(), crash_logfile_pid.c_str());
#endif
}

void log_hang(std::string write_file_n, std::string out_str_p,
              std::string out_str, std::string junk_file_of_args, int pid) {
#ifdef __unix__
  std::ostringstream pid_as_s;
  pid_as_s << pid;
  std::string output_logfile_pid =
      write_file_n + ".output." + pid_as_s.str().c_str() + ".ansvif.log";
#endif
  std::ofstream xml_output;
#ifdef __unix__
  xml_output.open(output_logfile_pid.c_str());
#endif
#ifdef _WIN32
  std::string output_logfile =
      write_file_n + ".output." + ".ansvif.log";
  xml_output.open(output_logfile.c_str());
#endif
  Writer writer(xml_output);
  writer.openElt("Command")
      .attr("run", "What the command hung with")
      .content(out_str_p.c_str())
      .closeElt();
  writer.openElt("Command")
      .attr("run_plain", "What the command hung with (plaintext)")
      .content(out_str.c_str())
      .closeElt();
  writer.openElt("File data")
      .attr("file", "File data used left here")
      .content(junk_file_of_args.c_str())
      .closeAll();
  xml_output.close();
}
