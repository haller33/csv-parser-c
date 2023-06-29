#define NOBUILD_IMPLEMENTATION
#include "./include/nobuild.h"

#define FOLDER_FILES_TEST "t"

#define FOLDER_FILES_SRC "src"

#define FOLDER_BIN_FILES "bin"

#define CFLAGS "-Wall", "-O0", "-Wextra", "-g", "-std=c99", "-pedantic"

#define CLIBS "resources/arena.c"

void build_file_src(const char *tool, const char *src_file) {

  if (!path_is_dir("bin")) {
    MKDIRS("bin");
  }

  const Cstr tool_path = PATH(src_file, tool);
  const Cstr bin_path = PATH(FOLDER_BIN_FILES, tool);

#ifndef _WIN32
  CMD("cc", CFLAGS, "-o", NOEXT(bin_path), CLIBS, tool_path);
#else
  CMD("cl.exe", "/Fe.\\bin\\", tool_path);
#endif
}

void build_src(void) {

  FOREACH_FILE_IN_DIR(tool, FOLDER_FILES_SRC, {
    if (ENDS_WITH(tool, ".c")) {
      build_file_src(tool, FOLDER_FILES_SRC);
    }
  });

  FOREACH_FILE_IN_DIR(tool, FOLDER_FILES_TEST, {
    if (ENDS_WITH(tool, ".c")) {
      build_file_src(tool, FOLDER_FILES_TEST);
    }
  });
}

int main(int argc, char **argv) {

  GO_REBUILD_URSELF(argc, argv);

  build_src();
  return 0;
}
