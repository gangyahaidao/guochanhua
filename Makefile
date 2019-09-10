CC = gcc
CPPFLAGS = 
CFLAGS = -Wall -o2 
INC_PATH = -I/usr/local/include

LIBPATH = -L.
LIBS = -lSKFAPI-mips64l-log

SRCS = $(wildcard *.c)
OBJS = $(patsubst %.c,%.o,$(SRCS))
TARGETS = main

all:$(TARGETS)

$(TARGETS):$(OBJS)
	$(CC) $^ -o $@ $(LIBPATH) $(LIBS)

$(OBJS):%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INC_PATH)

clean:
	$(RM) $(OBJS) $(TARGETS)

.PHONY: all clean
