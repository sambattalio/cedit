
CC=gcc

CFLAGS=-Wall -std=gnu99

TARGET=cedit

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm $(TARGET) *.o