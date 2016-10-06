#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>
#include <iostream>


int rand_me_plz(int rand_from, int rand_to);

int main (int argc, char *argv[]) {
    int ret_code = 1;
    /* int chmod(const char *pathname, mode_t mode); */
    if (argv[1] == "fchownat") {
        if (argc == 3) {
            ret_code = fchownat(argv[2], argv[3], argv[4], argv[5], argv[6]);
        }
    }
    if (argv[1] == "chmod") {
        if (argc == 6) {
            /* int chmod(const char *pathname, mode_t mode); */
            ret_code = chmod(argv[1], argv[2]);
        }
    }
    return (ret_code);
}
