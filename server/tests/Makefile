SOURCEDIR = source/
HEADERSDIR = headers/
FSOURCEDIR = ../source/
FHEADERSDIR = ../headers/
SOURCES = $(wildcard $(SOURCEDIR)*.c)
HEADERS = $(wildcard $(HEADERSDIR)*.h)
FSOURCESTEMP = $(wildcard $(FSOURCEDIR)*.c)
FSOURCES = $(FSOURCESTEMP:$(FSOURCEDIR)server.c=)
FHEADERS = $(wildcard $(FHEADERSDIR)*.h)

CC = gcc
CFLAGS:=

tests: $(HEADERS) $(SOURCES) $(FHEADERS) $(FSOURCES)
	$(CC) $(CFLAGS) -o $@ $(SOURCES) $(FSOURCES) -I$(HEADERSDIR) -I$(FHEADERSDIR)

clean:
	rm -f *~ $(SOURCEDIR)*~ $(HEADERSDIR)*~ tests

clear:
	rm -f *~ $(SOURCEDIR)*~ $(HEADERSDIR)*~
