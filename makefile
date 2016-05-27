ALL=compress
CC=g++
CFLAGS=-Wall
LDFLAGS=

all: $(ALL)

$(ALL): compress.o
	$(CC) -o $@ $(LDFLAGS) $^
	
%.o: %.cpp
	$(CC) -o $@ $(CFLAGS) -c $<
	
clean:
	rm -f $(ALL)
	rm -f *.o