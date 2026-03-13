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

all: fourier_series

.PHONY: printmakehelp_and_reminder
printmakehelp_and_reminder: fourier_series.cc Makefile
	$(info  /******************************************************************************/)
	$(info  * task --> printmakehelp_and_reminder: fourier_series.cc Makefile          *)
	$(info  * $$@ ----> $@                                         *)
	$(info  * $$< --------------------------------> $<                   *)
	$(info  * $$^ --------------------------------> $^          *)
	$(info  /******************************************************************************/)

fourier_series: fourier_series.cc
	$(CXX) -o $@ $^ $(CXXFLAGS)

clean:
	rm -f fourier_series
	rm -f *~
	rm -f $(OUTLIB)*.o
