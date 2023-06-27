#include <stdio.h>
#include <string.h>
#include <assert.h>

#define ARENA_IMPLEMENTATION
#include "../resources/arena.h"

#define ORGMODEPARSERC_H_

#define MAX_ARENA 1024

static Arena default_arena = {0};
static Arena temporary_arena = {0};
static Arena *context_arena = &default_arena;

void*
context_alloc(size_t size)
{
    assert(context_arena);
    return arena_alloc(context_arena, size);
}

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
  
  // printf("Hello World");

  int* pt = (int*) context_alloc (512);

  for (int i = 0; i < 100; i++) {

    pt[i] = i;
    // print("%d\n", pt[i]);
  }

  arena_summary(&default_arena);

  arena_free(&default_arena);
  arena_free(&temporary_arena);

  fclose ( fp );
  return 0;
}
