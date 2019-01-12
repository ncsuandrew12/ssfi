PROJECT_ROOT = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

OBJS = err.o log.o ssfi.o

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
	$(CXX) -o $@ $^

%.o:	$(PROJECT_ROOT)%.cpp
	$(CXX) -c $(CFLAGS) $(CXXFLAGS) $(CPPFLAGS) -o $@ $<

%.o:	$(PROJECT_ROOT)%.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<

clean:
	rm -fr ssfi $(OBJS)
