#include "sample_First.h"
#include <dr_api.h>

static void print_stack_pointer() {
  void* p = NULL;
  printf("%p\n", (void*)&p);
}

static void dr_print_stack_pointer() {
  void* p = NULL;
  dr_printf("%p\n", (void*)&p);
}

static jint findMax
(JNIEnv *env, jobject obj, jint x, jint y)
{
  if (x == y) {
    return x * y;
  } else if (x > y) {
    return x;
  } else {
    return y;
  }
}

JNIEXPORT jint JNICALL Java_sample_First_findMax
(JNIEnv *env, jobject obj, jint x, jint y)
{
  printf("findMax called with env %p\n", env);
  printf("SP before dr_app_start: "); print_stack_pointer();
  dr_app_start();
  printf("SP after dr_app_start: "); print_stack_pointer();

  int result = findMax(env, obj, x, y);

  printf("SP before dr_app_stop: "); print_stack_pointer();
  dr_app_stop();
  printf("SP after dr_app_stop: "); print_stack_pointer();
  printf("returning from findMax\n");
  return result;
}

static JNIEnv *env = NULL;
static jclass First = NULL;
static jmethodID basicBlockCallbackID = NULL;

static void
basic_block_hook(app_pc start)
{
  dr_printf("Building basic block for %p\n", start);

  if (env == NULL || First == NULL || basicBlockCallbackID == NULL) {
    return;
  }
  dr_printf("SP in basic block hook: "); dr_print_stack_pointer();
  (*env)->CallStaticVoidMethod(env, First, basicBlockCallbackID, start);
}

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
  jint getEnvResult = (*vm)->GetEnv(vm, (void**)&env, JNI_VERSION_1_2);
  if (getEnvResult == JNI_OK) {
    printf("GetEnv() OK: %p\n", env);
  } else if (getEnvResult == JNI_EDETACHED) {
    printf("GetEnv() returned EDETACHED\n");
  } else if (getEnvResult == JNI_EVERSION) {
    printf("GetEnv() returned EVERSION\n");
  }
  First = (*env)->FindClass(env, "sample/First");
  if (NULL == First) {
    dr_printf("Cannot find class sample.First\n");
  } else {
    basicBlockCallbackID = (*env)->GetStaticMethodID(env, First, "basicBlockCallback", "(J)V");
    if (NULL == basicBlockCallbackID) {
      dr_printf("Cannot find method First.basicBlockCallback\n");
    }
  }

  dr_app_setup();
  void *context = dr_get_current_drcontext();
  printf("DR context %p\n", context);
  set_build_basic_block_fragment_hook(context, &basic_block_hook);

  return JNI_VERSION_1_2;
}
