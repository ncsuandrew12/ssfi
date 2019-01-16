PROJECT_ROOT = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

OBJS = counter.o dir_counter.o log.o pool.o queue.o reader.o ssfi.o util.o

LIBS = -pthread

CPPFLAGS += -std=c++11
ifeq ($(BUILD_MODE),debug)
	CFLAGS += -g
	CFLAGS += -D SSFI_DEBUG
else
	CFLAGS += -O2 -Wall -Werror
endif

all:	build ssfi

build:
	mkdir -p $@

ssfi:	$(addprefix build/, $(OBJS))
	$(CXX) -o $@ $(LIBS) $^

build/%.o:	src/%.cpp
	$(CXX) -c $(CFLAGS) $(CXXFLAGS) $(CPPFLAGS) -o $@ $(LIBS) $<

build/%.o:	src/%.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $(LIBS) $<

clean:
	rm -rf ssfi build
