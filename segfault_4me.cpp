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
using namespace std;


/*
Marshall Whittaker / oxagast
gcc segfault_4me.cpp -lstdc++ -lz -std=gnu++11 -L./gzstream -lgzstream -o segfault_4me
./segfault_4me commandname commandpath buffersize
*/


string RemoveChars(const string& source, const string& chars) {
 string result="";
 for (unsigned int i=0; i<source.length(); i++) {
   bool foundany=false;
   for (unsigned int j=0; j<chars.length() && !foundany; j++) {
     foundany=(source[i]==chars[j]);
   }
   if (!foundany) {
     result+=source[i];
   }
 }
return result;
}

int rand_me_plz (int rand_from, int rand_to) {
  random_device rd;
  default_random_engine generator(rd());   // seed random
  uniform_int_distribution<int> distribution(rand_from,rand_to);  // get a random
  auto roll = bind ( distribution, generator );  // bind it so we can do it multiple times
  return(roll());  
}

vector<string> get_flags(char* cmd) {
  string cmd_name(cmd);
  string filename;
  int man_num;
  vector<string> opt_vec;
  string m_num;
  for (man_num = 1; man_num <= 8; man_num++) {  // loop around the manpages
    m_num = to_string(man_num); // change the loop's integar to a string
    filename = "/usr/share/man/man" + m_num + "/" + cmd_name + "." + m_num + ".gz"; // put the filename back together
    char* chr_fn = strdup(filename.c_str()); // change the filename to a char pointer
    igzstream in(chr_fn);  //  gunzip baby
    string gzline;
    regex start_of_opt_1 ("^(\\.?\\\\?\\w{2} )*(\\\\?\\w{2} ?)*(:?\\.B )*((?:(?:\\\\-)+\\w+)(?:\\\\-\\w+)*).*"); // hella regex... why you do this to me manpage?
    smatch opt_part_1;
    regex start_of_opt_2 ("^\\.Op Fl (\\w+) Ar.*");
    smatch opt_part_2;
    regex start_of_opt_3 ("^\\\\fB(-.*)\\\\fB.*");
    smatch opt_part_3;
    while(getline(in, gzline)){
       if (regex_match(gzline, opt_part_1, start_of_opt_1)) {  // ring 'er out
        string opt_1 = opt_part_1[4];
        string opt_release = (RemoveChars(opt_part_1[4], "\\"));  // remove the fucking backslashes plz
        opt_vec.push_back(opt_release);
      }
      if (regex_match(gzline, opt_part_2, start_of_opt_2)) {
        string opt_2 = opt_part_2[1];
        opt_vec.push_back("-" + opt_2);
      }
      if (regex_match(gzline, opt_part_3, start_of_opt_3)) {
        string opt_3 = opt_part_3[1];
        opt_vec.push_back(opt_3);  
      }
    }
  }
  sort(opt_vec.begin(), opt_vec.end());
  opt_vec.erase(unique(opt_vec.begin(), opt_vec.end()), opt_vec.end());
  return(opt_vec);
}

string make_garbage(int trash, int buf, string user_junk) {
  int trash_num;
  string junk = "";
  string hex_stuff;
  if (trash == 0) {
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
    for (trash_num = 0; trash_num < buf; trash_num++) {
      junk = user_junk; // ditto for null
    }
  }
  if (trash == 3) {
    string rand_char;
    for (trash_num = 0; trash_num < 256; trash_num++) {
      const char* hexdigit = "0123456789abcdef";
      for(int hex_out_curl=0; hex_out_curl< 2; hex_out_curl+=2) {
        char hexxy_1 = hexdigit[rand_me_plz(0,16) % 16];  // get 1 part of hex digit
        char hexxy_2 = hexdigit[rand_me_plz(0,16) % 16];  // get 2 part of hex digit
        rand_char = to_string(hexxy_1) + to_string(hexxy_2);  // put them together
        string byte = rand_char.substr(hex_out_curl,2);  // pull out one hex
        char chr = (char) (int)strtol(byte.c_str(), NULL, 16);  // get the char
        junk.push_back(chr);  // put it into junk
      }
    }
  }
  if (trash == 4) {
    for (trash_num = 0; trash_num < buf; trash_num++) {
      junk = user_junk;
    }
  }
  return(junk);  
}

string execer(char* sys_c) {
  FILE* pipe = popen(sys_c, "r");  // open a pipe
  if (!pipe) return "ERROR";
  char buffer[128];
  string result = "";
  while(!feof(pipe)) {
    if(fgets(buffer, 128, pipe) != NULL) result += buffer;  // put whatever comes out in buffer
  }
  pclose(pipe);
  return (result); // return it
}

void write_junk_file (int junk_num, int buf, string user_junk) {
  ofstream junk_file;
  junk_file.open ("junk");
  junk_file << make_garbage(junk_num, buf, user_junk);
  junk_file.close();
}



int main (int argc, char* argv[]) {
  if ((argc < 4 ) || (argc > 5)) {
    cerr << "Useage:" << endl;
    cerr << argv[0] << " [command name] [command path] [buffer size] abcd" << endl;
    cerr << argv[0] << " deluser /bin/deluser 2048 abcd" << endl;
    exit (1);
  }
  char* the_cmd = argv[1];
  char* the_path = argv[2];
  string cmd_str(argv[1]);
  string path_str(argv[2]);
  int buf_size = atoi(argv[3]);
  int segged = 0;
  vector<string> opts = get_flags(the_cmd);
//  vector<string> junk_opts;
  string user_j;
//  string sys_str;
  if (argc == 5) {
    string user_j(argv[4]);
  }
  if (argc == 4) {
    string user_j = "";
  }
  

  while (segged != 1) {

    vector<string> junk_opts;
    for( int cmd_flag_l=0; cmd_flag_l < opts.size(); cmd_flag_l++) {  // loop around the options
      if (rand_me_plz(0,1) == 1) {   // roll tha die
	junk_opts.push_back(opts.at(cmd_flag_l));  // put the random arg in the vector
      }
    }
    string sys_str = cmd_str + " ";
    for( vector<string>::const_iterator junk_opt = junk_opts.begin(); junk_opt != junk_opts.end(); ++junk_opt) { // loop through the vector of junk opts
      sys_str = sys_str + *junk_opt + " " + make_garbage(rand_me_plz(0,4), buf_size, user_j) + " ";  // add options and garbage
    }
    junk_opts.clear();
    junk_opts.shrink_to_fit();
    sys_str = path_str + " " + sys_str + " 2>&1";  // make with path and garbage interactive
    char* sys_chr = strdup(sys_str.c_str());  // duplicate point to the string of chars
    istringstream is_it_segfault(execer(sys_chr)); // run it and grab stdout and stderr
    free(sys_chr);  // free up memory from sys_chr so we don't OOM later
    string sf_line;
    while (getline(is_it_segfault, sf_line)) {
      regex sf_reg ("Segmentation fault"); // regex for the sf
      smatch sf;
      if (regex_match( sf_line, sf, sf_reg)) {  // match segfault
	cout << "Segfaulted with: " << sys_str << endl; 
	segged = 1;
      }
    }
  }
  return 0;  
}
