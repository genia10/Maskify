SOURCEDIR = source/
SOURCES = $(wildcard $(SOURCEDIR)*.c)
HEADERSDIR = headers/
HEADERS = $(wildcard $(HEADERSDIR)*.h)
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = server
CFLAGS  = -O3 -march=native -mtune=native

server: $(OBJECTS) $(HEADERS)
	gcc $(CFLAGS) -o $@ $(OBJECTS)

.c.o: $(SOURCES)
	gcc $(CFLAGS) -c -o $@ $< -Iheaders

clean:
	rm $(EXECUTABLE) $(SOURCEDIR)*.o $(SOURCEDIR)*~ *~

clear:
	rm $(SOURCEDIR)*.o $(SOURCEDIR)*~ *~
