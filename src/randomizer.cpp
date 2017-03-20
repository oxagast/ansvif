/*
 * ansvif
 * A Not So Very Intelligent Fuzzer
 * Marshall Whittaker / oxagast
 */

//    __ _  _  __   ___  __  ____ ____ 
//   /  ( \/ )/ _\ / __)/ _\/ ___(_  _)
//  (  O )  (/    ( (_ /    \___ \ )(  
//   \__(_/\_\_/\_/\___\_/\_(____/(__)

#include <random>
#include <functional>
int rand_me_plz(int rand_from, int rand_to) {
#ifdef _WIN32
  int first = rand_to-(rand_from-1);
  int rolled = rand()% first - rand_from;
#endif
#ifdef __unix__
    std::random_device rd;
  std::default_random_engine generator(rd()); // seed random
  std::uniform_int_distribution<int> distribution(rand_from,
                                                  rand_to); // get a random
  auto roll = std::bind(distribution,
                        generator); // bind it so we can do it multiple times
int rolled = roll();
#endif
  return (rolled);
}
