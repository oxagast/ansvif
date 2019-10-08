/*
 * ansvif
 * A Not So Very Intelligent Fuzzer
 * Marshall Whittaker / oxagast
 *
 *
 *   __ _  _  __   ___  __  ____ ____
 *  /  ( \/ )/ _\ / __)/ _\/ ___(_  _)
 * (  O )  (/    ( (_ /    \___ \ )(
 *  \__(_/\_\_/\_/\___\_/\_(____/(__)
 */

int main() {
  /* this is bad, i'm calling on a null pointer */
  char *assigned_null = 0;
  int null_pointer = *assigned_null;
}
