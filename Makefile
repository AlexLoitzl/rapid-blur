CC := cc

TARGET := rapid-blur

CFLAGS=-O3 -march=native -fopenmp -Wall -Wextra -lX11

OBJS := i3lock-fancy-rapid.o

all: $(TARGET)

$(TARGET): $(TARGET).c $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(TARGET).c -o $(TARGET)

clean:
	$(RM) $(TARGET) $(OBJS)

i3lock-fancy-rapid.o: i3lock-fancy-rapid.c
	$(CC) $(CFLAGS) -c i3lock-fancy-rapid.c
