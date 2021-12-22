SOURCEDIR = source/
HEADERSDIR = headers/
FSOURCEDIR = ../source/
FHEADERSDIR = ../headers/
# SOURCES = $(wildcard $(SOURCEDIR)*.c)
SOURCES = $(SOURCEDIR)munit.c $(SOURCEDIR)tests.c $(SOURCEDIR)error.c $(SOURCEDIR)commands.c $(SOURCEDIR)funcs.c $(SOURCEDIR)masks.c $(SOURCEDIR)prepare_socket.c $(SOURCEDIR)prepare_signals.c $(SOURCEDIR)prepare_child_processes.c $(SOURCEDIR)talk.c $(SOURCEDIR)function.c
HEADERS = $(wildcard $(HEADERSDIR)*.h)
# FSOURCESTEMP = $(wildcard $(FSOURCEDIR)*.c)
# FSOURCES = $(FSOURCESTEMP:$(FSOURCEDIR)server.c=)
FSOURCES = $(FSOURCEDIR)funcs.c $(FSOURCEDIR)commands.c $(FSOURCEDIR)error.c $(FSOURCEDIR)masks.c $(FSOURCEDIR)prepare_socket.c $(FSOURCEDIR)prepare_signals.c $(FSOURCEDIR)prepare_child_processes.c $(FSOURCEDIR)talk.c
FHEADERS = $(wildcard $(FHEADERSDIR)*.h)

CC = gcc
CFLAGS:=

tests: $(HEADERS) $(SOURCES) $(FHEADERS) $(FSOURCES)
	$(CC) $(CFLAGS) -o $@ $(SOURCES) $(FSOURCES) -I$(HEADERSDIR) -I$(FHEADERSDIR)

tests2: $(HEADERS) $(SOURCES) $(FHEADERS) $(FSOURCES)
	$(CC) $(CFLAGS) -o $@ $(SOURCEDIR)munit.c $(SOURCEDIR)tests.c $(SOURCEDIR)error.c $(SOURCEDIR)commands.c $(FSOURCEDIR)funcs.c $(FSOURCEDIR)commands.c $(FSOURCEDIR)error.c -I$(HEADERSDIR) -I$(FHEADERSDIR)

clean:
	rm -f *~ $(SOURCEDIR)*~ $(HEADERSDIR)*~ tests

clear:
	rm -f *~ $(SOURCEDIR)*~ $(HEADERSDIR)*~
