SHELL := /bin/bash

CROSS_COMPILE=

AR = $(CROSS_COMPILE)ar
RL = $(CROSS_COMPILE)ranlib
AS = $(CROSS_COMPILE)as
CC = $(CROSS_COMPILE)gcc
CXX = $(CROSS_COMPILE)g++ 
LD = $(CROSS_COMPILE)ld
OD = $(CROSS_COMPILE)objdump
OC = $(CROSS_COMPILE)objcopy
ST = $(CROSS_COMPILE)strip

INSTALL=install

FULL_DIR := $(shell pwd)
OBJDIR=objs

LIBINCLUDE= -I./include \
	     		-I$(XERCES_DIST)/include \
            -I$(BOOST_DIST)/include 

CFLAGS +=  -g -O2 -fno-strict-aliasing


LIBS = 	 -lrt -lpthread -lm -lc \
         -L$(XERCES_DIST)/lib -lxerces-c \
         -L$(BOOST_DIST)/lib  -lboost_system -lboost_filesystem

vpath %.cpp src

SOURCES=main.cpp mediaDataParser.cpp 
OBJECTS=$(SOURCES:%.cpp=objs/%.o)

EXECUTABLE=paxus_media_util

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJDIR) $(OBJECTS)
	$(CXX) $(CFLAGS) $(LDFLAGS) $(OBJECTS) -o $@  $(ARCS) $(LIBS)

objs/%.o:%.cpp
	$(CXX) $(CFLAGS) $(LIBINCLUDE) -c $< -o $@
	$(CXX) -MM $(CFLAGS) $(LIBINCLUDE)  $< > objs/$*.d
	@mv -f objs/$*.d objs/$*.d.tmp
	@sed -e 's|.*:|objs/$*.o:|' < objs/$*.d.tmp > objs/$*.d
	@sed -e 's/.*://' -e 's/\\$$//' < objs/$*.d.tmp | fmt -1 | \
	  sed -e 's/^ *//' -e 's/$$/:/' >> objs/$*.d
	@rm -f objs/$*.d.tmp

-include $(OBJECTS:.o=.d)

$(OBJDIR):
	( mkdir ${OBJDIR}; )
	
clean:
	rm -rf ${OBJDIR} $(EXECUTABLE)
