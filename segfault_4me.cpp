#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include "gzstream/gzstream.h"
#include <algorithm>
#include <vector>
#include <sys/time.h>
#include <random>
#include <cstdlib>
#include <climits>
#include <sstream>
#include <new>
#include "pstreams/pstream.h"
#include <cstring>


/*
Marshall Whittaker / oxagast
gcc segfault_4me.cpp -lstdc++ -lz -std=gnu++11 -L./gzstream -lgzstream -o segfault_4me
./segfault_4me commandname commandpath buffersize
*/


std::string remove_chars(const std::string& source, const std::string& chars) {
  std::string result="";
  for (unsigned int i=0; i<source.length(); i++) {
    bool foundany=false;
    for (unsigned int j=0; j<chars.length() && !foundany; j++) {
      foundany=(source[i]==chars[j]);
    }
    if (!foundany) {
      result+=source[i];
    }
  }
  return (result);
}


int rand_me_plz (int rand_from, int rand_to) {
  std::random_device rd;
  std::default_random_engine generator(rd());   // seed random
  std::uniform_int_distribution<int> distribution(rand_from,rand_to);  // get a random
  auto roll = std::bind ( distribution, generator );  // bind it so we can do it multiple times
  return(roll());  
}


char fortune_cookie () {
    std::string rand_char;
    std::string byte;
    char chr;
      const char* hexdigit = "0123456789abcdef";  // hex character makeup
      for(int hex_out_curl=0; hex_out_curl< 2; hex_out_curl+=2) {
        char hexxy_1 = hexdigit[rand_me_plz(0,16) % 16];  // get 1 part of hex digit
        char hexxy_2 = hexdigit[rand_me_plz(0,16) % 16];  // get 2 part of hex digit
        rand_char = std::to_string(hexxy_1) + std::to_string(hexxy_2);  // put them together
        byte = rand_char.substr(hex_out_curl,2);  // pull out one hex
        chr = (char) (int)strtol(byte.c_str(), NULL, 16);  // get the char
        }
    return(chr);
}


std::vector<std::string> get_flags_man(char* cmd) {
  std::string cmd_name(cmd);
  std::string filename;
  int man_num;
  std::vector<std::string> opt_vec;
  std::string m_num;
  for (man_num = 1; man_num <= 8; man_num++) {  // loop around the manpages
    m_num = std::to_string(man_num); // change the loop's integar to a string
    filename = "/usr/share/man/man" + m_num + "/" + cmd_name + "." + m_num + ".gz"; // put the filename back together
    char* chr_fn = strdup(filename.c_str()); // change the filename to a char pointer
    igzstream in(chr_fn);  //  gunzip baby
    std::string gzline;
    std::regex start_of_opt_1 ("^(\\.?\\\\?\\w{2} )*(\\\\?\\w{2} ?)*(:?\\.B )*((?:(?:\\\\-)+\\w+)(?:\\\\-\\w+)*).*"); // hella regex... why you do this to me manpage?
    std::smatch opt_part_1;
    std::regex start_of_opt_2 ("^\\.Op Fl (\\w+) Ar.*");
    std::smatch opt_part_2;
    std::regex start_of_opt_3 ("^\\\\fB(-.*)\\\\fB.*");
    std::smatch opt_part_3;
    while(std::getline(in, gzline)){
       if (std::regex_match(gzline, opt_part_1, start_of_opt_1)) {  // ring 'er out
        std::string opt_1 = opt_part_1[4];
        std::string opt_release = (remove_chars(opt_part_1[4], "\\"));  // remove the fucking backslashes plz
	opt_vec.push_back(opt_release);
      }
      if (std::regex_match(gzline, opt_part_2, start_of_opt_2)) {
        std::string opt_2 = opt_part_2[1];
        opt_vec.push_back("-" + opt_2);
      }
      if (std::regex_match(gzline, opt_part_3, start_of_opt_3)) {
        std::string opt_3 = opt_part_3[1];
        opt_vec.push_back(opt_3);
      }
    }
  }
  std::sort(opt_vec.begin(), opt_vec.end());
  opt_vec.erase(unique(opt_vec.begin(), opt_vec.end()), opt_vec.end());
  return(opt_vec);
}


std::vector<std::string> get_flags_template(std::string filename) {
  int man_num;
  std::vector<std::string> opt_vec;
  std::string line;
  std::ifstream template_file (filename);
  if (template_file.is_open())
  {
    while (std::getline (template_file,line))
    {
      opt_vec.push_back(line);
    }
    template_file.close();
  }
  else { 
    std::cerr << "Could not open file..." << std::endl;
    exit (1);
  }
  return(opt_vec);  
}


std::string make_garbage(int trash, int buf) {
  int trash_num;
  std::string junk = "";
  std::string hex_stuff;
  std::string user_junk;
  buf = buf-1;
  if (isatty(STDIN_FILENO)) {
    user_junk = "";
    if (trash == 0) {                                            // kosher
      for (trash_num = 0; trash_num < buf; trash_num++) {
        junk = "A" + junk; // put lots of As
      }
    }
    if (trash == 1) {
      for (trash_num = 0; trash_num < buf; trash_num++) {
        junk = "9" + junk; // yadda yadda
      }
    }
    if (trash == 2) {
      char fortune = fortune_cookie(); // ditto for random
      for (trash_num = 0; trash_num < buf; trash_num++) {
        junk = junk += fortune;
      }
    }
    if (trash == 3) {
      for (trash_num = 0; trash_num < buf; trash_num++) {
        junk = junk += fortune_cookie();
      }
    }
    if (trash == 4) {                                            // front
      for (trash_num = 0; trash_num < buf; trash_num++) {
        junk = "A" + junk; // put lots of As
      }
      junk = user_junk + junk;
      if (buf-user_junk.length() < junk.size()) junk = junk.substr(0,buf);
      else return ("OOR");
    }
    if (trash == 5) {
      for (trash_num = 0; trash_num < buf; trash_num++) {
        junk = "9" + junk; // yadda yadda
      }
      junk = user_junk + junk;
      if (buf-user_junk.length() < junk.size()) junk = junk.substr(0,buf);
      else return ("OOR");
    }
    if (trash == 6) {
      char fortune = fortune_cookie(); // ditto for random
      for (trash_num = 0; trash_num < buf; trash_num++) {
        junk = junk += fortune;
      }
      junk = user_junk + junk;
      if (buf-user_junk.length() < junk.size()) junk = junk.substr(0,buf);
      else return ("OOR");
    }
    if (trash == 7) {
      for (trash_num = 0; trash_num < buf; trash_num++) {
        junk = junk += fortune_cookie();
      }
      junk = user_junk + junk;
      if (buf-user_junk.length() < junk.size()) junk = junk.substr(0,buf);
      else return ("OOR");
    }
    if (trash == 8) {
      for (trash_num = 0; trash_num < buf; trash_num++) {  // back
        junk = "A" + junk; // put lots of As
      }
      junk = junk + user_junk;
      if (buf-user_junk.length() < junk.size()) junk = junk.substr(junk.length()-buf);
      else return ("OOR");
    }
    if (trash == 9) {
      for (trash_num = 0; trash_num < buf; trash_num++) {
        junk = "9" + junk; // yadda yadda
      }
      junk = junk + user_junk;
      if (buf-user_junk.length() < junk.size()) junk = junk.substr(junk.length()-buf);
      else return ("OOR");
    }
    if (trash == 10) {
      char fortune = fortune_cookie(); // ditto for random
      for (trash_num = 0; trash_num < buf; trash_num++) {
        junk = junk += fortune;
      }
      junk = junk + user_junk;
      if (buf-user_junk.length() < junk.size()) junk = junk.substr(junk.length()-buf);
      else return ("OOR");
    }
    if (trash == 11) {
      for (trash_num = 0; trash_num < buf; trash_num++) {
        junk = junk += fortune_cookie();
      }
      junk = junk + user_junk;
      if (buf-user_junk.length() < junk.size()) junk = junk.substr(junk.length()-buf);
      else return ("OOR");
    }
  }
  else {
    getline(std::cin, user_junk);
      if (trash == 0) {                                            // kosher
        for (trash_num = 0; trash_num < buf; trash_num++) {
          junk = "A" + junk; // put lots of As
        }
      }
      if (trash == 1) {
        for (trash_num = 0; trash_num < buf; trash_num++) {
          junk = "9" + junk; // yadda yadda
        }
      }
      if (trash == 2) {
        char fortune = fortune_cookie(); // ditto for random
        for (trash_num = 0; trash_num < buf; trash_num++) {
          junk = junk += fortune;
        }
      }
      if (trash == 3) {
        for (trash_num = 0; trash_num < buf; trash_num++) {
          junk = junk += fortune_cookie();
        }
      }
      if (trash == 4) {                                            // front
        for (trash_num = 0; trash_num < buf; trash_num++) {
          junk = "A" + junk; // put lots of As
        }
        junk = user_junk + junk;
        if (buf-user_junk.length() < junk.size()) junk = junk.substr(0,buf);
        else return ("OOR");
      }
      if (trash == 5) {
        for (trash_num = 0; trash_num < buf; trash_num++) {
          junk = "9" + junk; // yadda yadda
        }
        junk = user_junk + junk;
        if (buf-user_junk.length() < junk.size()) junk = junk.substr(0,buf);
        else return ("OOR");
      }
      if (trash == 6) {
        char fortune = fortune_cookie(); // ditto for random
        for (trash_num = 0; trash_num < buf; trash_num++) {
          junk = junk += fortune;
        }
        junk = user_junk + junk;
        if (buf-user_junk.length() < junk.size()) junk = junk.substr(0,buf);
        else return ("OOR");
      }
      if (trash == 7) {
        for (trash_num = 0; trash_num < buf; trash_num++) {
          junk = junk += fortune_cookie();
        }
        junk = user_junk + junk;
        if (buf-user_junk.length() < junk.size()) junk = junk.substr(0,buf);
        else return ("OOR");
      }
      if (trash == 8) {
        for (trash_num = 0; trash_num < buf; trash_num++) {  // back
          junk = "A" + junk; // put lots of As
        }
        junk = junk + user_junk;
        if (buf-user_junk.length() < junk.size()) junk = junk.substr(junk.length()-buf);
        else return ("OOR");
      }
      if (trash == 9) {
        for (trash_num = 0; trash_num < buf; trash_num++) {
          junk = "9" + junk; // yadda yadda
        }
        junk = junk + user_junk;
        if (buf-user_junk.length() < junk.size()) junk = junk.substr(junk.length()-buf);
        else return ("OOR");
      }
      if (trash == 10) {
        char fortune = fortune_cookie(); // ditto for random
        for (trash_num = 0; trash_num < buf; trash_num++) {
          junk = junk += fortune;
        }
        junk = junk + user_junk;
        if (buf-user_junk.length() < junk.size()) junk = junk.substr(junk.length()-buf);
        else return ("OOR");
      }
      if (trash == 11) {
        for (trash_num = 0; trash_num < buf; trash_num++) {
          junk = junk += fortune_cookie();
        }
        junk = junk + user_junk;
        if (buf-user_junk.length() < junk.size()) junk = junk.substr(junk.length()-buf);
        else return ("OOR");
      }
    }
  
  return(junk);
}

std::string execer(std::string the_cmd_str) {
  std::vector<std::string> errors;
  redi::ipstream in(the_cmd_str + " >&2", redi::pstreambuf::pstderr); // gotta put them to stderr
  std::string errmsg;                                                      // some os thing
  while (std::getline(in, errmsg)) {
    errors.push_back(errmsg);
  }
  if (errors.size() > 0) {
  return (errors[errors.size() - 1]);
  }
  else return ("NOSEG");                                                // damn it...
}


int match_seg(int buf_size, std::vector<std::string> opts, std::string path_str) {
  bool segged = false;
  while (segged != true) {

    std::vector<std::string> junk_opts;
    for( int cmd_flag_l = 0; cmd_flag_l < opts.size(); cmd_flag_l++) {  // loop around the options
      if (rand_me_plz(0,1) == 1) {   // roll tha die
	junk_opts.push_back(opts.at(cmd_flag_l));  // put the random arg in the vector
      }
    }
    std::string sys_str = path_str + " ";
    for( std::vector<std::string>::const_iterator junk_opt = junk_opts.begin(); junk_opt != junk_opts.end(); ++junk_opt) { // loop through the vector of junk opts
      std::string oscar = make_garbage(rand_me_plz(0,11),buf_size);
      if (oscar != "OOR") {
        sys_str = sys_str + *junk_opt + " " + oscar + " ";  // add options and garbage
      }
    }
    junk_opts.clear();
    junk_opts.shrink_to_fit();
    std::istringstream is_it_segfault(execer(sys_str)); // run it and grab stdout and stderr

    std::string sf_line;
    while (std::getline(is_it_segfault, sf_line)) {
      std::regex sf_reg ("(.*Segmentation fault.*|.*core dump.*)"); // regex for the sf
      std::smatch sf;
      if (regex_match(sf_line, sf, sf_reg)) {  // match segfault
	std::cout << "Segfaulted with: " << sys_str << std::endl; 
	segged = true;
	exit (0);
      }
    }
  }
  return(0);
}

/*
// not yet used...

void write_junk_file (int junk_num, int buf, string user_junk) {
  ofstream junk_file;
  junk_file.open ("junk");
  junk_file << make_garbage(junk_num, buf, user_junk);
  junk_file.close();
}
*/


int main (int argc, char* argv[]) {
  if ((argc < 4 ) || (argc > 5)) {
    std::cerr << "Useage:" << std::endl;
    std::cerr << argv[0] << " [command name] [command path] [buffer size]" << std::endl;
    std::cerr << argv[0] << " deluser /bin/deluser 2048" << std::endl;
    exit (1);
  }
  char* the_man = argv[2];
  std::string template_str(argv[2]);
  std::string template_type_str(argv[1]);
  std::string path_str(argv[3]);
  int buf_size = std::atoi(argv[4]);

  std::vector<std::string> opts;
  if (template_type_str == "-m") {
    opts = get_flags_man(the_man);
  }
  else if (template_type_str == "-t") {
    opts = get_flags_template(template_str);
  }
  else {
    std::cerr << "Please specify:" << std::endl << "  -m for a manpage followed by the command name" 
    << std::endl << "  -t followed by a template filename." << std::endl;
    exit (1);
  }
  match_seg(buf_size, opts, path_str);
  return (0);  
}
