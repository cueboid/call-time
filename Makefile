CC      = gcc
CFLAGS  = -Wall -g
TARGET  = call-time
SRC     = main.c
OBJ     = main.o

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

main.o: $(SRC)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJ)

install: $(TARGET)
	cp $< /usr/bin

uninstall:
	rm -f /usr/bin/$(TARGET)
