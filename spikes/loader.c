#include <dlfcn.h>
#include <stdio.h>

#ifdef __cplusplus
  extern "C" {
#endif


void load_and_execute(const char *libname)
{
  printf("Hello from loader\n");

  void *handle = dlopen(libname, RTLD_LAZY | RTLD_LOCAL);
  if (NULL == handle) {
    fprintf(stderr, "Error opening library %s: %s\n", libname, dlerror());
    return;
  }

  void (*probe)() = (void (*)())dlsym(handle, "probe");
  if (NULL != probe) {
    printf("Symbol `probe` found. Calling it!\n");
    probe();
  } else {
    printf("Didn't find symbol `probe`\n");
  }

  dlclose(handle);
}

#ifdef __cplusplus
};
#endif
