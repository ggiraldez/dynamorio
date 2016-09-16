#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include "dr_api.h"

static volatile int block_count = 0;

static dr_emit_flags_t
event_basic_block(void *drcontext, void *tag, instrlist_t *bb,
                  bool for_trace, bool translating)
{
  block_count++;
  dr_fprintf(STDERR, "bb_event for %p\n", tag);
  return DR_EMIT_DEFAULT;
}


static void
event_exit()
{
  dr_printf("Final block count %d\n", block_count);
}

int main(int argc, char *argv[])
{
  int result;

  result = dr_app_setup();
  if (result != 0) {
    fprintf(stderr, "DynamoRIO setup failed\n");
    exit(1);
  }

  dr_fprintf(STDERR, "Registering basic block event handler\n");
  dr_register_bb_event(event_basic_block);
  dr_register_exit_event(event_exit);

  // yield control to DynamoRIO
  dr_app_start();

  printf("Hello from test1!\n");
  if (dr_app_running_under_dynamorio()) {
    printf("Running under DynamoRIO\n");
  } else {
    printf("NOT running under DynamoRIO\n");
  }

  void *handle = dlopen("./loader.so", RTLD_NOW | RTLD_LOCAL);
  if (NULL == handle) {
    fprintf(stderr, "Error loading loader: %s\n", dlerror());
    exit(1);
  }

  void (*load_and_execute)(const char*) = (void(*)(const char*))dlsym(handle, "load_and_execute");
  if (NULL != load_and_execute) {
    printf("Found load_and_execute\n");
    load_and_execute("./target.so");
  } else {
    printf("Cannot find load_and_execute!\n");
  }

  dlclose(handle);

  dr_app_stop();
  if (dr_app_running_under_dynamorio()) {
    printf("STILL running under DynamoRIO\n");
  } else {
    printf("Not running under DynamoRIO anymore\n");
  }

  dr_app_cleanup();

  printf("Final block count %d\n", block_count);
  return 0;
}
