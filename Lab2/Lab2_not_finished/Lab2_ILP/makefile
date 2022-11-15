CXXPROC = g++
CPPFLAGS = -g -O3 -std=c++14
INCLUDE = -I ./inc

PROG_FILES_TO_LINK = main.o src/io/input.o src/ds/layout.o src/ds/ilpsolver.o
PROG_TARGET = Lab2

$(PROG_TARGET): $(PROG_FILES_TO_LINK)
	$(CXXPROC) $(CPPFLAGS) $(INCLUDE) -o $(PROG_TARGET) $^

src/ds/ilpsolver.o: src/ds/ilpsolver.cpp
	$(CXXPROC) $(CCFLAGS) $(CPPFLAGS) $(INCLUDE) -o $@ -c $^

%.o : %.cpp 
	$(CXXPROC) $(CPPFLAGS) $(INCLUDE) -o $@ -c $^

clean:
	rm -f $(PROG_TARGET)
	rm -f */*/*.o */*.o *.o



# Uncomment for CPLEX_Studio124
SYSTEM     = x86-64_linux
LIBFORMAT  = static_pic
CPLEXDIR      = ../CPLEX/path/cplex
CONCERTDIR    = ../CPLEX/path/concert

################

CCC = g++ -std=c++14 -g

CCOPT = -m64 -O3 -fPIC -fno-strict-aliasing -fexceptions -DNDEBUG -DIL_STD -D_GNU_SOURCE

CPLEXBINDIR   = $(CPLEXDIR)/bin/$(BINDIST)
CPLEXLIBDIR   = $(CPLEXDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
CONCERTLIBDIR = $(CONCERTDIR)/lib/$(SYSTEM)/$(LIBFORMAT)

CCLNFLAGS = -L$(CPLEXLIBDIR) -lilocplex -lcplex -L$(CONCERTLIBDIR) -lconcert -lm -pthread -ldl

# -ldl needed for CPLEX version > 12.6


CONCERTINCDIR = $(CONCERTDIR)/include
CPLEXINCDIR   = $(CPLEXDIR)/include

CCFLAGS = $(CCOPT) -I$(CPLEXINCDIR) -I$(CONCERTINCDIR) 
