#include <stdio.h>
#include "../inc/bencode.h"

int main (int argc, const char* argv[]) {
  int i ;
  for (i = 0; i < argc; i++) {
    printf("%s\n", argv[i]);
  }
  b_encode* bp = b_encode_init(argv[1]);
  b_encode_print(bp);
  return 0;
}

