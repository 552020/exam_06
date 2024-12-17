#include <stdio.h>

int main(int argc, char **argv) {
  printf("Hello World\n");
  if (argc == 1) {
    printf("Wrong number of arguments\n");
    return (1);
  }
  return (0);
}
