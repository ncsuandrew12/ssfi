PROJECT_ROOT = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

OBJS = counter.o dir_counter.o err.o log.o reader.o ssfi.o util.o

CPPFLAGS += -std=c++11
ifeq ($(BUILD_MODE),debug)
	CFLAGS += -g
	CFLAGS += -D SSFI_DEBUG
else ifeq ($(BUILD_MODE),run)
	CFLAGS += -O2
#else
#	$(error Build mode $(BUILD_MODE) not supported by this Makefile)
endif

all:	ssfi

ssfi:	$(OBJS)
	$(CXX) -o $@ -pthread $^

%.o:	$(PROJECT_ROOT)%.cpp
	$(CXX) -c $(CFLAGS) $(CXXFLAGS) $(CPPFLAGS) -o $@ -pthread $<

%.o:	$(PROJECT_ROOT)%.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ -pthread $<

clean:
	rm -fr ssfi $(OBJS)
