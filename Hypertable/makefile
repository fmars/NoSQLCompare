HT_HOME=/opt/hypertable/current
LIBSIGAR=-lsigar-amd64-linux
CC=g++
CFLAGS=-c -g -Wall -D_REENTRANT -std=c++11 -I$(HT_HOME)/include \
       -I$(HT_HOME)/include/thrift
LDFLAGS=-g -rdynamic -L$(HT_HOME)/lib -lHyperThrift -lHyperCommon -lHypertable \
	-lthrift -levent -lboost_system -lboost_program_options \
	-lboost_filesystem $(LIBSIGAR) -lre2 -lcurses -ltcmalloc_minimal \
	-lpthread


all: client_test

client_test: client_test.o
	$(CC) client_test.o $(LDFLAGS) -o client_test

client_test.o: client_test.cc
	$(CC) $(CFLAGS) client_test.cc

clean:
	rm -rf *o client_test
