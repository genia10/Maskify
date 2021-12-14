SOURCES = $(wildcard *.c)
HEADERS = $(wildcard *.h)
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = server
CFLAGS  = -O3 -march=native -mtune=native

server: $(OBJECTS) $(HEADERS)
	gcc $(CFLAGS) -o $@ $(OBJECTS)

.c.o: $(SOURCES)
	gcc $(CFLAGS) -c -o $@ $<

clean:
	rm $(EXECUTABLE) *.o *~

clear:
	rm *.o *~
