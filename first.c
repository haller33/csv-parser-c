#define NOBUILD_IMPLEMENTATION
#include "./include/nobuild.h"

#define CFLAGS "-Wall", "-Wextra", "-std=c99", "-pedantic"

void build_file(const char *tool) {
  
  if (!path_is_dir("bin")) {
    MKDIRS("bin");
  }
  Cstr tool_path = PATH("src", tool);
  Cstr bin_path = PATH("bin", tool);
#ifndef _WIN32
  CMD("cc", CFLAGS, "-o", NOEXT(bin_path), tool_path);
#else
  CMD("cl.exe", "/Fe.\\bin\\", tool_path);
#endif
}

void build_src(void) {

  FOREACH_FILE_IN_DIR(tool, "src", {
      if (ENDS_WITH(tool, ".c")) {
	build_file(tool);
      }
    });
}

int
main (int argc, char **argv) {

    GO_REBUILD_URSELF(argc, argv);

    build_src (  );
    return 0;
}


