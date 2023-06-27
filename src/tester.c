#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CSVPARSERC_IMPLEMENTATION
#include "lib/csv_parser.h"

int
main (int argc, char *argv[]) {

  if (argc < 2) {
    printf("no file specify\ntry out %s <file_name>.csv", argv[0]);
    return 127;
  }

  read_csv_file_path(argv[1]);
  return 0;
}
