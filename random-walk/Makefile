program_NAME := RandomWalk
program_C_SRCS := $(wildcard *.c) $(wildcard */*.c)
program_CXX_SRCS := $(wildcard *.cpp) $(wildcard */*.cpp)
program_C_OBJS := ${program_C_SRCS:.c=.o}
program_CXX_OBJS := ${program_CXX_SRCS:.cpp=.o}
program_OBJS := $(program_C_OBJS) $(program_CXX_OBJS)
program_INCLUDE_DIRS := 
program_LIBRARY_DIRS :=
program_LIBRARIES :=
program_FLAGS := -Wall -O3 -DNDEBUG

CPPFLAGS += $(foreach includedir,$(program_INCLUDE_DIRS),-I$(includedir))
CPPFLAGS += $(program_FLAGS)
LDFLAGS += $(foreach librarydir,$(program_LIBRARY_DIRS),-L$(librarydir))
LDFLAGS += $(foreach library,$(program_LIBRARIES),-l$(library))

.PHONY: all clean distclean exec

all: $(program_NAME)

$(program_NAME): $(program_OBJS)
	$(LINK.cc) $(program_OBJS) -o $(program_NAME) 

clean:
	@- $(RM) $(program_NAME)
	@- $(RM) $(program_OBJS)

distclean: clean

exec:
	./$(program_NAME) && cd data && gnuplot -p plot.gp && cd ..
