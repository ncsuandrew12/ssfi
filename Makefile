PROJECT_ROOT = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

OBJS = counter.o dir_counter.o log.o pool.o reader.o ssfi.o ssfi_ex.o util.o

CPPFLAGS += -std=c++11
ifeq ($(BUILD_MODE),debug)
	CFLAGS += -g
	CFLAGS += -D SSFI_DEBUG
else
	CFLAGS += -O2
endif

all:	build ssfi

build:
	mkdir -p $@

ssfi:	$(addprefix build/, $(OBJS))
	$(CXX) -o $@ -pthread $^

build/%.o:	src/%.cpp
	$(CXX) -c $(CFLAGS) $(CXXFLAGS) $(CPPFLAGS) -o $@ -pthread $<

build/%.o:	src/%.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ -pthread $<

clean:
	rm -rf ssfi build
