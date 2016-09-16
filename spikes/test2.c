#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "dr_api.h"

static void* getEIP()
{
  return __builtin_return_address(0);
};

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

static void
basic_block_hook(app_pc start)
{
  dr_printf("Building basic block for %p\n", start);
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
  /* build_basic_block_hack = &basic_block_hook; */
  dr_register_bb_event(event_basic_block);
  dr_register_exit_event(event_exit);

  void *context = dr_get_current_drcontext();
  printf("DR context %p\n", context);
  set_build_basic_block_fragment_hook(context, &basic_block_hook);

  printf("EIP before yielding to DynamoRIO: %p\n", getEIP());

  // yield control to DynamoRIO
  dr_app_start();

  printf("EIP after yielding to DynamoRIO: %p\n", getEIP());

  printf("Hello from test2!\n");
  if (dr_app_running_under_dynamorio()) {
    printf("Running under DynamoRIO\n");
  } else {
    printf("NOT running under DynamoRIO\n");
  }

  srand(time(NULL));
  int x = rand();
  if (x > RAND_MAX / 2) {
    printf("Big!\n");
  } else {
    printf("Small\n");
  }

  int a = 1, b = 1, c = 0;
  int i;
  for (i = 0; i < 10; i++) {
    c += a + b;
    a = b;
    b = c;
  }
  printf("Result is %d\n", c);

  printf("EIP before stopping DynamoRIO: %p\n", getEIP());
  dr_app_stop();
  printf("EIP after stopping DynamoRIO: %p\n", getEIP());

  if (dr_app_running_under_dynamorio()) {
    printf("STILL running under DynamoRIO\n");
  } else {
    printf("Not running under DynamoRIO anymore\n");
  }

  dr_app_cleanup();

  printf("Final block count %d\n", block_count);
  return 0;
}
