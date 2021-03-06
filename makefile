
COMPILIER=g++

CFLAGS=-g -Wall -std=c++11 -O3 

LDFLAGS=

HEADERS= include/basic_types.h	\
	include/bmpio.h	\
	include/utility.h	\
	include/basic_geometry.h	\
	include/basic_tracer.h	\
	include/basic_render.h	\
	include/tga_input.h	\
	include/basic_renderer.h	\
	include/basic_texture.h	\
	include/view.h	\
	include/object_descripter.h	\
	include/bounding_volume_hierachy.h

SOURCES= main.cpp	\
	lib/basic_geometry.cc 	\
	lib/bmpio.cc	\
	lib/basic_tracer.cc	\
	lib/tga_input.cc	\
	lib/basic_renderer.cc	\
	lib/object_descripter.cpp	\
	lib/bounding_volume_hierachy.cc	\
	lib/basic_types.cc

EXECUTABLE=main

all: $(SOURCES) $(HEADERS)
	$(COMPILIER) $(CFLAGS) $(SOURCES) -o $(EXECUTABLE)

clean:
	rm $(EXECUTABLE)

gprof: $(SOURCES) $(HEADERS)
	$(COMPILIER) $(CFLAGS) -pg $(SOURCES) -o $(EXECUTABLE)
