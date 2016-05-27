ALL=compress
CC=gcc
CFLAGS=-Wall
LDFLAGS=

all: $(ALL)

$(ALL): compress.o
	$(CC) -o $@ $(LDFLAGS) $^
	
%.o: %.c
	$(CC) -o $@ $(CFLAGS) -c $<
	
clean:
	rm -f $(ALL)
	rm -f *.o
