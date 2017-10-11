/*
 * ansvif
 * A Not So Very Intelligent Fuzzer
 * Marshall Whittaker / oxagast
 *
 *
 *    __ _  _  __   ___  __  ____ ____ 
 *  /  ( \/ )/ _\ / __)/ _\/ ___(_  _)
 * (  O )  (/    ( (_ /    \___ \ )(  
 *  \__(_/\_\_/\_/\___\_/\_(____/(__)
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
void func(char **argv) {
  printf("running strcpy...\n");
  char arr[16];
  strcpy(arr, argv[1]);
}
int main(int argc, char *argv[]) {
  if(argc == 2) {
    func(argv);
  }
  return(0);
}
