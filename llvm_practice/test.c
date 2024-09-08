#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  unsigned int number, temp, droot = 0;
  number = atol(argv[1]);
  temp = number;
  while (temp != 0) {
    int digit = temp % 10;
    droot += digit;
    temp /= 10;
    if (temp == 0 && droot > 9) {
      temp = droot;
      droot = 0;
    }
  }

  return 0;
}