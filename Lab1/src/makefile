CXXPROC = g++
CPPFLAGS = -std=c++11 -O3

PROG_TARGET = Lab1
PROG_FILES_TO_LINK = main.o io/io.o cs/cs.o

$(PROG_TARGET): $(PROG_FILES_TO_LINK)
	$(CXXPROC) $(CPPFLAGS) -o $(PROG_TARGET) $(PROG_FILES_TO_LINK)

%.o : %.cpp 
	$(CXXPROC) $(CPPFLAGS) -o $*.o -c $*.cpp

clean:
	rm -f $(PROG_TARGET)
	rm -f */*.o *.o