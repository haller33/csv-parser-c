#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define ARENA_DEFAULT_CAPACITY (2840 * 1000)
// #define JUST_MALLOC false
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

  size_t columns_c = _csvc_count_columns(arr_vet_chr);
  printf("%d\n", (int)columns_c);

  csvc_free_context(ctx);

  csv_adt *ctx_neo = csvc_dump_csv(argv[1]);

  for(size_t i = 0; i < ctx_neo->_rows_count; i++) {
    for(int j = 0; j < (int)ctx_neo->_columns_count; j++) {
      printf("%s ",ctx_neo->csv_raw_data[i][j]);
    }
    printf("\n");
  }

  csvc_free_context(ctx_neo);


  return 0;
}
