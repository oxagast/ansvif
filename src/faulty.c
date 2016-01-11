#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void func(char **argv) {
  char arr[16];
  strcpy(arr, argv[2]);
  return;
}
void spawnme() {
  setuid(0);
  int stat = system("/bin/bash -p");
}
int main(int argc, char *argv[]) {
  int blah = 0;
  if ((argc > 2) && (argc < 4)) func(argv);
  if (blah == 1)
    spawnme();
  else
    return 1;
}
