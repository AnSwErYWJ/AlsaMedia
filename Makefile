PLAYER_SRC := ./src/player.c
RECORDER_SRC := ./src/recorder.c
PUBLIC_SRC := ./src/public.c
All_SRC = $(PLAYER_SRC) $(RECORDER_SRC) $(PUBLIC_SRC)

PLAYER_OBJS := $(PLAYER_SRC:.c=.o)
RECORDER_OBJS := $(RECORDER_SRC:.c=.o)
PUBLIC_OBJS := $(PUBLIC_SRC:.c=.o)
All_OBJS := $(PLAYER_OBJS) $(RECORDER_OBJS) $(PUBLIC_OBJS)

PLAYER_TARGET := ./aplayer
RECORDER_TARGET := ./arecorder
ALL_TARGET := $(PLAYER_TARGET) $(RECORDER_TARGET)

CC := gcc
PLUS := g++
RM := -rm -rf
CFLAGS := -Wall -O2 -m64 -D_GUN_SOURCE
LDFLAGS = -lasound
CPPFLAGS = -I ./include/

#############################

.PHONY: all player recorder clean

all: player recorder

player : $(PLAYER_TARGET)
$(PLAYER_TARGET): $(PLAYER_SRC)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $(PUBLIC_SRC) -o $(PUBLIC_OBJS)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $^ -o $(PLAYER_OBJS)
	$(PLUS) $(LDFLAGS) $(PLAYER_OBJS) $(PUBLIC_OBJS) -o $@

recorder : $(RECORDER_TARGET)
$(RECORDER_TARGET): $(RECORDER_SRC)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $(PUBLIC_SRC) -o $(PUBLIC_OBJS)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $^ -o $(RECORDER_OBJS)
	$(PLUS) $(LDFLAGS) $(RECORDER_OBJS) $(PUBLIC_OBJS) -o $@

clean:
	$(RM) $(All_OBJS) $(ALL_TARGET)

