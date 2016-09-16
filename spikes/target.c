#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

void probe()
{
  printf("Hello from the target library!\n");
}

#ifdef __cplusplus
};
#endif
