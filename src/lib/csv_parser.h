
#ifndef CSVPARSERC_H_
#define CSVPARSERC_H_

#include "assert.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARENA_IMPLEMENTATION
#include "../../resources/arena.h"

#define ORGMODEPARSERC_H_

#ifndef CUSTOM_ALLOC
#define CUSTOM_ALLOC // thread safe, no shared
#define CSV_ALLOC _arena_context_alloc_noshare
#endif

// #define MIN_ARENA 1048576 // 2^20 bytes

#define MAX_HEADER_NAME 80
#define MAX_NAME_CELL 80

typedef struct _csv_adt {
  char **csv_columns_names;
  char **csv_raw_data;
  int csv_current_count_row;
  int _columns_count;
  int _max_name_cell;
  bool _full_data;
  Arena *_context_arena;
} csv_adt;

void *_arena_context_alloc_noshare(csv_adt* adt, size_t size);

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

char * //
_csvc_parser_line(char *buff);

void * //
_csvc_parser_colum(char *buff, int column_num);

#endif // CSVPARSERC_H_

#ifdef CSVPARSERC_IMPLEMENTATION

void * // thread safe std alloc
_arena_context_alloc_noshare(csv_adt *adt, size_t size) {
  assert(adt->_context_arena && "arena not pass");
  return arena_alloc(adt->_context_arena, size);
}

char ** // ["1", "chat", "csv", "0"]
read_idx_row(csv_adt *adt, int line) {
  assert(adt);
  assert(line);
}

char * // "1"
item_idx_row_colum(csv_adt *adt, int line, int colum) {
  assert(adt);
  assert(line);
  assert(colum);
}

char ** // ["city", "fortal", "maracanau"]
item_idx_colum(csv_adt *adt, int colum) {
  assert(adt);
  assert(colum);
}

csv_adt * // read all data to memory
dump_csv(char *file_path) {
  // assert(file_path);

  size_t read;
  char *line = NULL;
  size_t len = 0;

  FILE *fp = fopen(file_path, "ro");

  if (fp == NULL) {
    printf("file not open correcly :: %s", file_path);
    return (csv_adt*)NULL;
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
init_read_file_path(char *file_name) {

  // csv_adt* adt_new = &(csv_adt)malloc(sizeof(csv_adt));


  assert(file_name);
}

void //
free_csv_data(csv_adt *adt) {
  assert(adt);
}

int //
read_csv_file_path(char *name_with_path) {
  assert(name_with_path);
}

#endif // CSVPARSERC_IMPLEMENTATION
