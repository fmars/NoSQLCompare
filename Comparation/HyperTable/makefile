HT_HOME=/opt/hypertable/current
LIBSIGAR=-lsigar-amd64-linux
CC=g++
CFLAGS=-c -g -Wall -D_REENTRANT -std=c++11 -I$(HT_HOME)/include \
       -I$(HT_HOME)/include/thrift
LDFLAGS=-g -rdynamic -L$(HT_HOME)/lib -lHyperThrift -lHyperCommon -lHypertable \
	-lthrift -levent -lboost_system -lboost_program_options \
	-lboost_filesystem $(LIBSIGAR) -lre2 -lcurses -ltcmalloc_minimal 

all: insert query

insert:  insert.o
	$(CC) insert.o $(LDFLAGS) -o insert

insert.o: insert.cpp
	$(CC) $(CFLAGS) insert.cpp 

query :  query.o
	$(CC) query.o $(LDFLAGS) -o query  

query.o: query.cpp
	$(CC) $(CFLAGS) query.cpp 

clean:
	rm -rf *.o insert query *~
