CC:=gcc 

TARGET:=rapid-blur

CFLAGS:=-O3 -march=native -fopenmp -Wall -Wextra -Ithird-party

LDFLAGS:=-lm

OBJS :=i3lock-fancy-rapid.o

all: $(TARGET)

$(TARGET): $(TARGET).c $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(TARGET).c -o $(TARGET) $(LDFLAGS)

clean:
	$(RM) $(TARGET) $(OBJS)

i3lock-fancy-rapid.o: third-party/i3lock-fancy-rapid.c
	$(CC) $(CFLAGS) -c third-party/i3lock-fancy-rapid.c

install:
	ln -s $(shell pwd)/$(TARGET) /usr/bin/rapid-blur
