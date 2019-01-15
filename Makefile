PROJECT_ROOT = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

OBJS = counter.o dir_counter.o log.o reader.o ssfi.o ssfi_ex.o util.o

CPPFLAGS += -std=c++11
ifeq ($(BUILD_MODE),debug)
	CFLAGS += -g
	CFLAGS += -D SSFI_DEBUG
else
	CFLAGS += -O2
endif

all:	bin ssfi

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
