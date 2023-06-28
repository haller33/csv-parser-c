
#ifndef CSVPARSERC_H_
#define CSVPARSERC_H_

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARENA_IMPLEMENTATION
#include "../../resources/arena.h"

#ifndef CUSTOM_ALLOC
#define CUSTOM_ALLOC
#define CSV_ALLOC _arena_context_alloc_noshare
#endif

// #define MIN_ARENA 1048576 // 2^20 bytes

#define MAX_HEADER_NAME 80
#define MAX_NAME_CELL 80

#define SEPARATION_CARACTER ','

typedef struct _csv_adt {
  char **csv_columns_names;
  char **csv_raw_data;
  int csv_current_count_row;
  int _columns_count;
  int _max_name_cell;
  bool _full_data;
  FILE *_pdp;
  Arena *_context_arena;
} csv_adt;

void * //
_arena_context_alloc_noshare(csv_adt *adt, size_t size);

char ** // ["1", "chat", "csv", "0"]
csvc_read_idx_row(csv_adt *adt, int line);

char * // "1"
csvc_item_idx_row_colum(csv_adt *adt, int line, int colum);

char ** // ["city", "fortal", "maracanau"]
csvc_item_idx_colum(csv_adt *adt, int colum);

csv_adt * // read all data to memory
csvc_dump_csv(char *file_path);

csv_adt * //
csvc_init_read_file_path(char *file_name);

void //
csvc_free_context(csv_adt *adt);

char ** //
_csvc_parser_line(csv_adt *adt, char *buff);

void * //
_csvc_parser_colum(char *buff, int column_num);

char ** //
_csvc_alloc_chr_array(csv_adt *adt, size_t arr_size);

size_t //
_csvc_get_idx_chr(char *buff, char chr);

size_t //
_csvc_count_chr_on_str(char *str, char chr);

#endif // CSVPARSERC_H_

#ifdef CSVPARSERC_IMPLEMENTATION

void * // thread safe std alloc
_arena_context_alloc_noshare(csv_adt *adt, size_t size) {
  assert(adt->_context_arena && "arena not pass");
  return arena_alloc(adt->_context_arena, size);
}

char ** // ["1", "chat", "csv", "0"]
csvc_read_idx_row(csv_adt *adt, int line) {
  assert(adt);
  assert(line);
}

char * // "1"
csvc_item_idx_row_colum(csv_adt *adt, int line, int colum) {
  assert(adt);
  assert(line);
  assert(colum);
}

char ** // ["city", "fortal", "maracanau"]
csvc_item_idx_colum(csv_adt *adt, int colum) {
  assert(adt);
  assert(colum);
}

csv_adt * // read all data to memory
csvc_dump_csv(char *file_path) {
  // assert(file_path);

  size_t read;
  char *line = NULL;
  size_t len = 0;

  FILE *fp = fopen(file_path, "ro");

  if (fp == NULL) {
    printf("file not open correcly :: %s", file_path);
    return (csv_adt *)NULL;
  }

  // printf("Hello World\n");
  //

  while ((read = getline(&line, &len, fp)) != -1) {
    printf("Retrieved line of length %zu:\n", read);
    printf("%s", line);
  }

  // int *pti = (int *)CSV_ALLOC(512);

  fclose(fp);
}

csv_adt * //
csvc_init_read_file_path(char *file_name_path) {

  assert(file_name_path);

  FILE *fp = fopen(file_name_path, "ro");

  csv_adt *_adt_new = (csv_adt *)malloc(sizeof(csv_adt));
  Arena *_arena_new = (Arena *)malloc(sizeof(Arena));

  memset(_arena_new, 0, sizeof(Arena));

  _adt_new->_pdp = fp;

  _adt_new->_context_arena = _arena_new;

  return _adt_new;
}

void //
csvc_free_context(csv_adt *adt) {

  arena_free(adt->_context_arena);
  free(adt->_context_arena);

  fclose(adt->_pdp);

  free(adt);
}

size_t //
_csvc_get_idx_chr(char *buff, char chr) {

  size_t count = 0;

  while ((buff++[0] == chr) && (&buff[0] != NULL)) {
    count += 1;
  }

  return count;
}

char ** //
_csvc_alloc_chr_array(csv_adt *adt, size_t arr_size) {

  char **ret_char_arr = NULL;

  // ret_char_arr = (char**) malloc ( sizeof ( char** ) * arr_size );
  ret_char_arr = (char **)CSV_ALLOC(adt, sizeof(char **) * arr_size);

  for (size_t i = 0; i < arr_size; i++) {

    (ret_char_arr)[i] = (char *)CSV_ALLOC(adt, sizeof(char) * MAX_NAME_CELL);
    // (ret_char_arr)[i] = (char*) malloc ( sizeof ( char ) * MAX_NAME_CELL );

    memset(&(ret_char_arr)[i][0], 0, sizeof(char) * MAX_NAME_CELL);
  }

  return ret_char_arr;
}

size_t //
_csvc_count_chr_on_str(char *str, char chr) {

  size_t count = 0;

  while (strcmp(str++, "") && (str++ != NULL)) {
    if (str[0] == chr) {
      count += 1;
    }
  }

  return count;
}

char ** //
_csvc_parser_line(csv_adt *adt, char *buff) {

  char **new_str = NULL;

  char target = SEPARATION_CARACTER;

  size_t number_of_columns = _csvc_count_chr_on_str(buff, target);

  new_str = _csvc_alloc_chr_array(adt, number_of_columns);

  // int len = strlen(buff);
  size_t idx = 0;
  size_t current_idx = 0;
  size_t relative_count = 0;

  while (current_idx = _csvc_get_idx_chr(&buff[idx], target)) {

    if ((current_idx - idx) > MAX_NAME_CELL) {

      strncpy(&new_str[relative_count], &buff[idx], MAX_NAME_CELL);
    } else {

      strncpy(&new_str[relative_count], &buff[idx], current_idx - idx);
    }

    relative_count += 1;
    idx = current_idx;
  }

  return new_str;
}

void * //
_csvc_parser_colum(char *buff, int column_num) {
  assert(buff);
  assert(column_num);
}

#endif // CSVPARSERC_IMPLEMENTATION
