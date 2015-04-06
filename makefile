
COMPILIER=g++

CFLAGS=-g -Wall -std=c++0x -O2

LDFLAGS=

HEADERS= include/basic_types.h	\
	include/bmpio.h	\
	include/utility.h	\
	include/basic_geometry.h	\
	include/basic_tracer.h	\
	include/basic_render.h	

SOURCES= main.cpp	\
	lib/basic_geometry.cc 	\
	lib/bmpio.cc	\
	lib/basic_tracer.cc

EXECUTABLE=main

all: $(SOURCES) $(HEADERS)
	$(COMPILIER) $(CFLAGS) $(SOURCES) -o $(EXECUTABLE)

clean:
	rm $(EXECUTABLE)

gprof: $(SOURCES) $(HEADERS)
	$(COMPILIER) $(CFLAGS) -pg $(SOURCES) -o $(EXECUTABLE)
