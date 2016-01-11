#include <string.h>

int main(int argc, char *argv[]) {
  int i;
  char tmp[3];
  tmp[2] = '\0';
  if (argc > 1) {
    for (i = 2; i < strlen(argv[1]); i += 4) {
      strncpy(tmp, argv[1] + i, 2);
      printf("%c", (char)strtol(tmp, NULL, 16));
    }
  } else {
    printf("USAGE: printf.exe \\x41\\x42\n");
    return 1;
  }
  return 0;
}