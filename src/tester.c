#include <stdio.h>
#include <string.h>

#include "lib/csv_parser.h"

#define ORGMODEPARSERC_H_

int
main (int argc, char *argv[]) {

  if (argc < 2) {
    puts("no file specify");
    return 127;
  }

  FILE *fp = fopen(argv[1], "ro");
  
  if (fp == NULL) {
    printf ("file not open correcly :: %s", argv[1]);
    return 1;
  }
  
  printf("Hello World");
  


  fclose ( fp );
  return 0;
}
