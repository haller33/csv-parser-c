
#if !defined(CSVPARSERC_IMPLEMENTATION)
#define CSVPARSERC_IMPLEMENTATION
#endif

#ifndef CSVPARSERC_H_
#define CSVPARSERC_H_

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ARENA_IMPLEMENTATION
#include "../../resources/arena.h"

#ifndef CUSTOM_ALLOC
#define CUSTOM_ALLOC
#define CSV_ALLOC _arena_context_alloc_noshare
#endif // CUSTOM_ALLOC

#ifndef CUSTOM_MALLOC_C
#define CUSTOM_MALLOC_C malloc
#endif // CUSTOM_MALLOC_C

#ifndef CUSTOM_FREE_MALLOC_C
#define CUSTOM_FREE_MALLOC_C free
#endif // CUSTOM_FREE_MALLOC_C

#ifndef JUST_MALLOC
#define JUST_MALLOC false
#endif // JUST_MALLOC

#ifndef MAX_NAMES
#define MAX_HEADER_NAME 80
#define MAX_NAME_CELL 80
#endif // MAX_NAMES

#ifndef SEPARATION_CARACTER
#define SEPARATION_CARACTER ','
#define SPACE_CARACTER " "
#endif

typedef struct _csv_adt {
  char *_csvc_file_path;
  char *csv_current_line_buff;
  char **csv_columns_names;
  char ***csv_raw_data;

  size_t csv_current_count_row;
  size_t _columns_count;
  size_t _rows_count;
  size_t _max_name_cell;

  bool _full_data;
  bool _interation_is_on;
  FILE *_pdp;

  Arena *_context_arena;
} csv_adt;

void * //
_arena_context_alloc_noshare(csv_adt *adt, size_t size);

bool //
csvc_interate_increase_file(csv_adt *adt);

char * //
csvc_current_cell_by_index_of_column(csv_adt *adt, size_t idx);

char ** //
csvc_current_line(csv_adt *adt);

size_t //
csvc_current_line_number(csv_adt *adt);

char * //
csvc_stringfy_arr_char(csv_adt *adt, char **chr_arr);

char ** // ["1", "chat", "csv", "0"]
csvc_read_idx_row(csv_adt *adt, size_t line);

char * // "1"
csvc_item_idx_row_colum(csv_adt *adt, size_t line, size_t colum);

char ** // ["city", "fortal", "maracanau"]
csvc_item_idx_colum(csv_adt *adt, size_t colum);

csv_adt * // read all data to memory
csvc_dump_full_csv(char *file_path);

csv_adt * // read all data to memory
csvc_dump_full_csv_to_memory(csv_adt *adt);

csv_adt * //
csvc_init_read_file_path(char *file_name);

void //
csvc_close_file_path(csv_adt *adt);

void //
csvc_free_context(csv_adt *adt);

void //
_csvc_interate_over_item_now_cleanup(csv_adt *adt);

char ** //
_csvc_parser_line(csv_adt *adt, char *buff);

char ** //
_csvc_alloc_chr_array(csv_adt *adt, size_t arr_size);

size_t //
_csvc_get_idx_chr(char *buff, char chr);

// implement this, because some error on strncpy
// https://stackoverflow.com/questions/4593907/difference-between-strncpy-and-memcpy
// they verify some non termination caracter, or NULL, the rest of array, they fill with 'zero'
// probably this fill is causing memory problems on the Arena
char * //
_csvc_strcpy_to(char *dest, char *origin, size_t lenght_size);


size_t //
_csvc_count_chr_on_str(char *str, char chr);

size_t //
_csvc_count_columns(char **buff);

/// do not compile on android / termux
// size_t //
// getline(char **lineptr, size_t *n, FILE *stream);

#endif // CSVPARSERC_H_

#ifdef CSVPARSERC_IMPLEMENTATION

void * // thread safe std alloc
_arena_context_alloc_noshare(csv_adt *adt, size_t size) {
  assert(adt->_context_arena && "arena not pass");
  return arena_alloc(adt->_context_arena, size);
}

bool //
csvc_interate_increase_file(csv_adt *adt) {
  assert(adt);
  size_t read = 0;
  size_t len = 0;

  adt->_interation_is_on = true;

  read = getline(&adt->csv_current_line_buff, &len, adt->_pdp);

  if (read != (size_t)-1) {
    return true;
  } else {
    adt->_rows_count = adt->csv_current_count_row += 1;
    adt->_interation_is_on = false;

    return false;
  }

  return false;
}

bool
csvc_is_interating(csv_adt *adt) {
  assert(adt);

  return adt->_interation_is_on;
}

char * //
csvc_current_cell_by_index_of_column(csv_adt *adt, size_t idx) {
  // 'idx' can be zero index;
  assert(adt);

  char **current_item_line = csvc_current_line(adt);

  return current_item_line[idx];
}

char ** //
csvc_current_line(csv_adt *adt) {
  assert(adt);
  assert(adt->_pdp);
  assert(adt->csv_current_line_buff);

  char **buff_chr_arr = _csvc_parser_line(adt, adt->csv_current_line_buff);

  CUSTOM_FREE_MALLOC_C(adt->csv_current_line_buff);
  adt->csv_current_line_buff = NULL;

  if (adt->csv_current_count_row == 0) {

    adt->_columns_count = _csvc_count_columns(buff_chr_arr);
    adt->csv_columns_names = buff_chr_arr;
  }
  adt->csv_current_count_row += 1;

  return buff_chr_arr;
}

char * // implement this, because some memory errors
_csvc_strcpy_to(char *dest, char *origin, size_t lenght_size) {
  assert(dest);
  assert(origin);
  assert(lenght_size);
  assert(lenght_size > 0);

  for (size_t i = 0; i < lenght_size; ++i) {
    dest[i] = origin[i];
  }

  return dest;
}

char * //
csvc_stringfy_arr_char(csv_adt *adt, char **chr_arr) {
  assert(adt);
  assert(chr_arr);

  char *caracter_space = SPACE_CARACTER;
  size_t columns_size_digit = _csvc_count_columns(chr_arr);

  char *new_str_chr =
      (char *)CSV_ALLOC(adt, sizeof(char) * columns_size_digit * MAX_NAME_CELL +
                                 columns_size_digit + 1);

  int relative_counter = 0;
  for (size_t i = 0; i < columns_size_digit; i++) {
    char *tmp = chr_arr[i];
    if (!tmp) {
      break;
    }
    // arena_summary(adt->_context_arena);
    int size_str_chr = strlen(tmp);

    _csvc_strcpy_to(&new_str_chr[relative_counter], chr_arr[i], size_str_chr);
    _csvc_strcpy_to(&new_str_chr[relative_counter + size_str_chr], caracter_space, 1);

    relative_counter = relative_counter + size_str_chr + 1;
  }

  return new_str_chr;
}

size_t //
csvc_current_line_number(csv_adt *adt) {
  assert(adt);

  return adt->csv_current_count_row;
}

char ** // ["1", "chat", "csv", "0"]
csvc_read_idx_row(csv_adt *adt, size_t line) {
  assert(adt);
  assert(line);

  if (adt->_full_data) {
    assert(line < adt->_rows_count);
    return adt->csv_raw_data[line];
  }

  return (char **)NULL;
}

char * // "1"
csvc_item_idx_row_colum(csv_adt *adt, size_t line, size_t colum) {
  assert(adt);
  assert(line);
  assert(colum);

  if (adt->_full_data) {
    assert(colum < adt->_columns_count);
    assert(line < adt->_rows_count);
  }

  if (adt->_full_data) {

    return adt->csv_raw_data[line][colum];
  }

  return (char *)NULL;
}

char ** // ["city", "fortal", "maracanau"]
csvc_item_idx_colum(csv_adt *adt, size_t colum) {
  assert(adt);
  assert(colum);

  if (adt->_full_data) {
    assert(colum < adt->_columns_count);

    char **raw_colum = _csvc_alloc_chr_array(adt, adt->_rows_count);

    for (size_t i = 0; i < adt->_rows_count; i++) {
      raw_colum[i] = adt->csv_raw_data[i][colum];
    }

    return raw_colum;
  }

  return (char **)NULL;
}

size_t //
_csvc_count_lines(char *file_name_path) {
  assert(file_name_path);

  FILE *file;
  char ch;
  size_t count = 0;

  // Open the file in read mode
  file = fopen(file_name_path, "ro");

  // Check if the file exists
  if (file == NULL) {
    printf("File not found.\n");
    return 0;
  }

  // Count the lines
  while ((ch = fgetc(file)) != EOF) {
    if (ch == '\n') {
      count++;
    }
  }

  // Close the file
  fclose(file);

  return count;
}

csv_adt * // read all data to memory, dump into memory
csvc_dump_full_csv(char *file_path) {
  assert(file_path);

  csv_adt *ctx_adt = csvc_init_read_file_path(file_path);

  return csvc_dump_full_csv_to_memory(ctx_adt);
}

csv_adt * //
csvc_dump_full_csv_to_memory(csv_adt *adt) {
  assert(adt);

  if (adt->_interation_is_on) {
    return (csv_adt*) NULL;
  }

  size_t read;
  char *line_ptr = NULL;
  size_t len = 0;

  size_t file_lines_number = _csvc_count_lines(adt->_csvc_file_path);
  adt->_rows_count = file_lines_number;

  // actual data to be used for represent the CSV
  char ***raw_data =
      (char ***)CSV_ALLOC(adt, sizeof(char ***) * file_lines_number);

  read = getline(&line_ptr, &len, adt->_pdp);
  file_lines_number -= 1; // for header file, columns
  adt->csv_columns_names = _csvc_parser_line(adt, line_ptr);

  const size_t columns_count = _csvc_count_columns(adt->csv_columns_names);
  adt->_columns_count = columns_count;

  // for flag that the data aready has been dump to main memory
  adt->_full_data = true;

  size_t relative_count = 0;
  while ((read = getline(&line_ptr, &len, adt->_pdp)) != (size_t)-1) {

    raw_data[relative_count] = _csvc_parser_line(adt, line_ptr);
    relative_count += 1;
  }
  CUSTOM_FREE_MALLOC_C(line_ptr);

  adt->csv_raw_data = raw_data;

  return adt;
}

csv_adt * //
csvc_init_read_file_path(char *file_name_path) {

  assert(file_name_path);

  FILE *fp = fopen(file_name_path, "ro");

  if (fp == NULL) {
    printf("file not open correcly :: %s", file_name_path);
    assert(fp);
    return (csv_adt *)NULL;
  }

  csv_adt *_adt_new = (csv_adt *)CUSTOM_MALLOC_C(sizeof(csv_adt));
  Arena *_arena_new = (Arena *)CUSTOM_MALLOC_C(sizeof(Arena));

  memset(_arena_new, 0, sizeof(Arena));

  _adt_new->_csvc_file_path = file_name_path;
  _adt_new->csv_current_line_buff = NULL;
  _adt_new->csv_current_count_row = 0;
  _adt_new->_rows_count = 0;
  _adt_new->csv_raw_data = NULL;

  _adt_new->_interation_is_on = false;
  _adt_new->_full_data = false;
  _adt_new->_max_name_cell = MAX_NAME_CELL;
  _adt_new->_columns_count = 0;

  _adt_new->_pdp = fp;

  _adt_new->_context_arena = _arena_new;

  return _adt_new;
}

void //
csvc_close_file_path(csv_adt *adt) {
  assert(adt);
  if (adt->_pdp) {
    fclose(adt->_pdp);
  }
}

void //
csvc_free_context(csv_adt *adt) {
  assert(adt);

  arena_free(adt->_context_arena);
  CUSTOM_FREE_MALLOC_C(adt->_context_arena);
  if (adt->csv_current_line_buff) {
    CUSTOM_FREE_MALLOC_C(adt->csv_current_line_buff);
  }

  csvc_close_file_path(adt);

  CUSTOM_FREE_MALLOC_C(adt);
}

size_t //
_csvc_get_idx_chr(char *string, char c) {
  // c cam be '\0' caracter
  assert(string);

  char *str = strchr(string, c);

  if (str == NULL) {

    return strlen(string);
  }

  return (size_t)(str - string);
}

size_t //
_csvc_count_chr_on_str(char *str, char chr) {
  // chr can be a '\0' caracter
  assert(str);

  const char *p = str;
  size_t count = 0;

  do {

    if (*p == chr) {

      count++;
    }
  } while (*(p++));

  return count;
}

char ** //
_csvc_alloc_chr_array(csv_adt *adt, size_t arr_size) {
  assert(adt);
  assert(arr_size);
  assert(arr_size > 0);

  char **ret_char_arr = NULL;

#if JUST_MALLOC
  ret_char_arr = (char **)CUSTOM_MALLOC_C(sizeof(char **) * arr_size);
#else
  ret_char_arr = (char **)CSV_ALLOC(adt, sizeof(char **) * arr_size);
#endif

  for (size_t i = 0; i < arr_size; i++) {

#if JUST_MALLOC
    (ret_char_arr)[i] = (char *)CUSTOM_MALLOC_C(sizeof(char) * MAX_NAME_CELL);
#else
    (ret_char_arr)[i] = (char *)CSV_ALLOC(adt, sizeof(char) * MAX_NAME_CELL);
#endif

    memset(&(ret_char_arr)[i][0], 0, sizeof(char) * MAX_NAME_CELL);
  }

  return ret_char_arr;
}

char ** //
_csvc_parser_line(csv_adt *adt, char *buff) {

  assert(adt);
  assert(buff);

  char **new_str = NULL;
  const char target = SEPARATION_CARACTER;

  size_t number_of_columns = _csvc_count_chr_on_str(buff, target);
  number_of_columns += 1; // for the default 0 count, and last column
  new_str = _csvc_alloc_chr_array(adt, number_of_columns+2);

  // int len = strlen(buff);
  size_t idx = 0;
  size_t current_idx = 0;
  size_t relative_count = 0;

  while ((current_idx = _csvc_get_idx_chr(&buff[idx], target)) &&
         (relative_count < number_of_columns)) {

    if (current_idx > MAX_NAME_CELL) {

      _csvc_strcpy_to(new_str[relative_count], &buff[idx], MAX_NAME_CELL+1);
    } else {

      _csvc_strcpy_to(new_str[relative_count], &buff[idx], current_idx);
    }

    relative_count += 1;
    idx = idx + current_idx + 1;
  }

  new_str[number_of_columns+1] = NULL;

  return new_str;
}

size_t //
_csvc_count_columns(char **buff) {
  assert(buff);

  size_t count = 0;
  while (buff[count] != NULL) {
    count += 1;
  }
  if (count == 0) {
    return 0;
  }

  return count - 1;
}

#endif // CSVPARSERC_IMPLEMENTATION
