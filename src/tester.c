#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CSVPARSERC_IMPLEMENTATION
#include "lib/csv_parser.h"

int
main (int argc, char *argv[]) {

  if (argc < 2) {
    printf("no file specify\ntry out %s with <file_name>.csv", argv[0]);
    return 127;
  }

  csv_adt *ctx = csvc_init_read_file_path(argv[1]);

  char str[] = "some,text,data";
  char** arr_vet_chr = _csvc_parser_line(ctx, str);

  for(int i = 0; i < 3; i++) {
    printf("%s\n", arr_vet_chr[i]);
  }

  csvc_free_context(ctx);

  return 0;
}
