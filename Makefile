PROJECT_ROOT = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

BIN = $PROJECT_ROOT/bin
SRC = $PROJECT_ROOT/src

OBJS = counter.o dir_counter.o err.o log.o reader.o ssfi.o util.o

CPPFLAGS += -std=c++11
ifeq ($(BUILD_MODE),debug)
	CFLAGS += -g
	CFLAGS += -D SSFI_DEBUG
else
	CFLAGS += -O2
endif

all:	directories ssfi

directories: bin

bin:
	mkdir -p $@

ssfi:	$(addprefix bin/, $(OBJS))
	$(CXX) -o $@ -pthread $^

bin/%.o:	src/%.cpp
	$(CXX) -c $(CFLAGS) $(CXXFLAGS) $(CPPFLAGS) -o $@ -pthread $<

bin/%.o:	src/%.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ -pthread $<

clean:
	rm -rf ssfi bin
