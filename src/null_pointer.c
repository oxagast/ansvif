int main() {
  /* this is bad, i'm calling on a null pointer */
  char *assigned_null = 0;
  int null_pointer = *assigned_null;
}
