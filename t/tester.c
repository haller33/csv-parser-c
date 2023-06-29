#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define ARENA_DEFAULT_CAPACITY (2840 * 1000)
// #define JUST_MALLOC false
#define CSVPARSERC_IMPLEMENTATION
#include "../src/lib/csv_parser.h"

int //
main(int argc, char *argv[]) {

  if (argc < 2) {
    printf("no file specify\ntry out %s with <file_name>.csv", argv[0]);
    return 127;
  }

  char *file_name_path = argv[1];

  /*
   * on this example, we can see, how to read data from the file, on a
   * interative manner with this example, we can simple pass the file name and
   * then, interate over it, geting the data from eatch row and Cell .
   *
   */
  csv_adt *ctx_neo = csvc_init_read_file_path(file_name_path);

  while (csvc_interate_increase_file(ctx_neo)) {
    char **current_arr = csvc_current_line(ctx_neo);

    char *str = csvc_stringfy_arr_char(ctx_neo, current_arr);

    printf("%s\n", str);
  }

  csvc_free_context(ctx_neo);



  return 0;
}
