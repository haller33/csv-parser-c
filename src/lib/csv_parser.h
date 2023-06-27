
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
#define CUSTOM_ALLOC
#define CSV_ALLOC _arena_context_alloc // global alloc
// #define CSV_ALLOC _arena_context_alloc_noshare // thread safe, no shared
#endif

#define not_implemented crash(__PRETTY_FUNCTION__, " not implemented yet")

#define MIN_ARENA 1048576 // 2^20

static Arena default_arena = {0};
static Arena temporary_arena = {0};
static Arena *context_arena = &default_arena;

void *_arena_context_alloc(size_t size);

#define MAX_HEADER_NAME 80
#define MAX_NAME_CELL 80

typedef struct _csv_adt {
  char **header_names;
  int current_count_row;
  int columns_count;
  int max_name_cell;
  Arena *context_arena;
} csv_adt;

typedef struct _csv_adt_full {
  char **header_names;
  char **columns_names;
  int columns_count;
  int current_count_row;
  int max_name_cell;
  Arena *context_arena;
} csv_adt_full;

// typedef struct _csv_adt csv_adt;
// typedef struct _csv_adt_full csv_adt_full;

char ** // ["1", "chat", "csv", "0"]
read_idx_row(csv_adt *adt, int line);

char * // "1"
item_idx_row_colum(csv_adt *adt, int line, int colum);

char ** // ["city", "fortal", "maracanau"]
item_idx_colum(csv_adt *adt, int colum);

csv_adt_full *dump_csv(char *file_path); // read all data to memory

csv_adt *init_read_file_path(char *file_name);

void free_context(csv_adt *adt);

char *_csv_parser_line(char *buff);

void *_csv_parser_colum(char *buff, int column_num);

void *_csv_parser_line_adt(csv_adt *adt, char *line);

void *_csv_parser_colum_adt(csv_adt *adt, char *column_name);

#endif // CSVPARSERC_H_

#ifdef CSVPARSERC_IMPLEMENTATION

void *_arena_context_alloc_noshare(csv_adt *adt, size_t size) {
  assert(adt->context_arena && "arena not initialized");
  return arena_alloc(adt->context_arena, size);
}

void *_arena_context_alloc(size_t size) {
  assert(context_arena && "arena not initialized");
  return arena_alloc(context_arena, size);
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

csv_adt_full *dump_csv(char *file_path) { // read all data to memory
  // assert(file_path);

  size_t read;
  char *line = NULL;
  size_t len = 0;

  FILE *fp = fopen(file_path, "ro");

  if (fp == NULL) {
    printf("file not open correcly :: %s", file_path);
    return 1;
  }

  // printf("Hello World\n");
  //

  while ((read = getline(&line, &len, fp)) != -1) {
    printf("Retrieved line of length %zu:\n", read);
    printf("%s", line);
  }

  int *pti = (int *)CSV_ALLOC(512);

  fclose(fp);
}

csv_adt *init_read_file_path(char *file_name) { assert(file_name); }

void free_csv_data(csv_adt *adt) { assert(adt); }

void *_csv_parser_line_adt(csv_adt *adt, char *line) {
  assert(adt);
  assert(line);
}

void *_csv_parser_colum_adt(csv_adt *adt, char *column_name) {
  assert(adt);
  assert(column_name);
}

int read_csv_file_path(char *name_with_path) { assert(name_with_path); }

#endif // CSVPARSERC_IMPLEMENTATION
