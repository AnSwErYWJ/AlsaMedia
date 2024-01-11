PLAYER_SRC := ./src/player.c
RECORDER_SRC := ./src/recorder.c
INFO_SRC := ./src/info.c
All_SRC = $(PLAYER_SRC) $(RECORDER_SRC) $(INFO_SRC)

PLAYER_OBJS := $(PLAYER_SRC:.c=.o)
RECORDER_OBJS := $(RECORDER_SRC:.c=.o)
INFO_OBJS := $(INFO_SRC:.c=.o)
All_OBJS := $(PLAYER_OBJS) $(RECORDER_OBJS) $(INFO_OBJS)

PLAYER_TARGET := ./player
RECORDER_TARGET := ./recorder
INFO_TARGET := ./info
ALL_TARGET := $(PLAYER_TARGET) $(RECORDER_TARGET) $(INFO_TARGET)

CC := gcc
PLUS := g++
RM := -rm -rf
CFLAGS := -O3 -fvisibility=hidden -ffunction-sections -fstack-protector-strong -Wall -Werror -I ./include/ -g
LDFLAGS = -Wl,--gc-sections -lasound

#############################

.PHONY: all player recorder info clean

all: player recorder info

player : $(PLAYER_TARGET)
$(PLAYER_TARGET): $(PLAYER_SRC)
	$(CC) $(CFLAGS) -c $^ -o $(PLAYER_OBJS)
	$(PLUS) $(PLAYER_OBJS) $(CFLAGS) $(LDFLAGS) -o $@

recorder : $(RECORDER_TARGET)
$(RECORDER_TARGET): $(RECORDER_SRC)
	$(CC) $(CFLAGS) -c $^ -o $(RECORDER_OBJS)
	$(PLUS) $(RECORDER_OBJS) $(CFLAGS) $(LDFLAGS) -o $@

info : $(INFO_TARGET)
$(INFO_TARGET): $(INFO_SRC)
	$(CC) $(CFLAGS) -c $^ -o $(INFO_OBJS)
	$(PLUS) $(INFO_OBJS) $(CFLAGS) $(LDFLAGS) -o $@

clean:
	$(RM) $(All_OBJS) $(ALL_TARGET)

