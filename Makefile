ROOTCFLAGS    = $(shell $(ROOTSYS)/bin/root-config --cflags)
ROOTLIBS      = $(shell $(ROOTSYS)/bin/root-config --libs)
ROOTGLIBS     = $(shell $(ROOTSYS)/bin/root-config --glibs)

CXX  = g++
CXX += -I./

CXXFLAGS  = -g -Wall -fPIC -Wno-deprecated
CXXFLAGS += $(ROOTCFLAGS)
CXXFLAGS += $(ROOTLIBS)
CXXFLAGS += $(ROOTGLIBS)
CXXFLAGS += -fconcepts

#----------------------------------------------------#

all: polygon_chord

.PHONY: printmakehelp_and_reminder
printmakehelp_and_reminder: polygon_chord.cc Makefile
	$(info  /******************************************************************************/)
	$(info  * task --> printmakehelp_and_reminder: polygon_chord.cc Makefile          *)
	$(info  * $$@ ----> $@                                         *)
	$(info  * $$< --------------------------------> $<                   *)
	$(info  * $$^ --------------------------------> $^          *)
	$(info  /******************************************************************************/)

polygon_chord: polygon_chord.cc
	$(CXX) -o $@ $^ $(CXXFLAGS)

clean:
	rm -f polygon_chord
	rm -f *~

