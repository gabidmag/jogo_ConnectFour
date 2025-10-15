CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g

OBJS = main.o utils.o Game.o AI.o

TARGET = connect4

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c utils.h Game.h AI.h
	$(CC) $(CFLAGS) -c main.c

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c

Game.o: Game.c Game.h
	$(CC) $(CFLAGS) -c Game.c

AI.o: AI.c AI.h Game.h
	$(CC) $(CFLAGS) -c AI.c

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean