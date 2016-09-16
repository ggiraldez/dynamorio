JAVA_HOME = /usr/lib/jvm/java-8-openjdk-amd64/
CFLAGS = -Wall -g -O0 -rdynamic -DLINUX -DX86_64 -fno-stack-protector
JNI_CFLAGS = -D_REENTRANT -fPIC
INCLUDES = -I../build/include
JNI_INCLUDES = -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/linux
LDFLAGS = -g -z now
LIBS = -ldl -L../build/lib64/release -ldynamorio
PLUGIN_FLAGS = -shared -fPIC

all: test1 test2 loader.so target.so libfirst.so

loader.so: loader.c
	$(CC) $(CFLAGS) $(INCLUDES) loader.c -o loader.so $(LDFLAGS) $(PLUGIN_FLAGS)

target.so: target.c
	$(CC) $(CFLAGS) $(INCLUDES) target.c -o target.so $(LDFLAGS) $(PLUGIN_FLAGS)

test1.o: test1.c
	$(CC) $(CFLAGS) $(INCLUDES) -c test1.c

test1: test1.o
	$(CC) $(LDFLAGS) -o test1 test1.o $(LIBS)

test2.o: test2.c
	$(CC) $(CFLAGS) $(INCLUDES) -c test2.c

test2: test2.o
	$(CC) $(LDFLAGS) -o test2 test2.o $(LIBS)

sample_First.h: sample/First.java
	javah -jni -o sample_First.h sample.First

sample_First.o: sample_First.c sample_First.h
	$(CC) $(CFLAGS) $(JNI_CFLAGS) $(INCLUDES) $(JNI_INCLUDES) -c $< -o $@

libfirst.so: sample_First.o
	$(CC) $(LDFLAGS) $(PLUGIN_FLAGS) -o $@ $+ $(LIBS)

clean:
	rm -f *.o test1 test2 loader.so target.so libfirst.so

.PHONY: clean
