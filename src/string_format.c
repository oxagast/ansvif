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

int main(int argc, char **argv) {
  char buffer[64];
  int x = 1;
  snprintf(buffer, sizeof(buffer), argv[1]);
  buffer[sizeof(buffer) - 1] = 0;
  int howmuch = strlen(buffer);
  printf("Buffer:(%d) \nData:%s\n", howmuch, buffer);
  return 0;
}
