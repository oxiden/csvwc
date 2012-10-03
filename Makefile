CC = gcc
CFLAGS = -Wall -O2
BINFILE = csvwc
SRC = csvwc.c debugger.c debugger.h
csvwc:
	$(CC) $(CFLAGS) -o $(BINFILE) $(SRC)
debug:
	$(CC) $(CFLAGS) -DDEBUG -o $(BINFILE) $(SRC)
clean:
	if [ -e $(BINFILE) ]; then rm $(BINFILE); fi
