#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

  /**
   *
   * On this example, we interate over a one column of
   * the csv file.
   * this is done to simplify the need to use another variable
   *
   */
  csv_adt *ctx = csvc_init_read_file_path(file_name_path);

  while (csvc_interate_increase_file(ctx)) {
    char *current_chr = csvc_current_cell_by_index_of_column(ctx, 0);

    printf("%s\n", current_chr);
  }

  csvc_free_context(ctx);


  ///
  /// Example with multiples interators at same time.
  ///

  csv_adt *ctx_two = csvc_init_read_file_path(file_name_path);

  size_t relative_count = 0;
  while (csvc_interate_increase_file(ctx_two)) {

    if ((relative_count % 2) == 0) {
      char **current_arr = csvc_current_line(ctx_two);

      size_t num_line = csvc_current_line_number(ctx_two);

      size_t item = atoi(current_arr[1]) / num_line ;

      printf("second column %d\n", (int)item);
    } else {
      /// you cannot call the same interator at the same time
      //  without refreshing interate_increase_file
      //
      char *current_chr = csvc_current_cell_by_index_of_column(ctx_two, 0);

      printf("first column %d\n", atoi(current_chr));
    }

    relative_count += 1;
  }

  csvc_free_context(ctx);


  return 0;
}
