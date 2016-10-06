#include <random>

int rand_me_plz(int rand_from, int rand_to) {
  int first = rand_to-(rand_from-1);
  int roll = rand()% first - rand_from;
  return (roll);
}
