CC = gcc
CFLAGS = -Wall -O2
BINFILE = csvwc
SRC = csvwc.c debugger.c
HDR = debugger.h
OBJ = $(SRC:.c=.o)

%.o: %.c $(HDR)
	$(CC) $(CFLAGS) -c $< -o $@

csvwc: $(OBJ)
	$(CC) $(CFLAGS) -o $(BINFILE) $(OBJ)

debug: $(OBJ)
	$(CC) $(CFLAGS) -DDEBUG -o $(BINFILE) $(OBJ)

clean:
	rm -f $(BINFILE)
	rm -f $(OBJ)
