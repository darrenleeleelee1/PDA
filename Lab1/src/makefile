CXXPROC = g++
CPPFLAGS = -g -std=c++11

PROG_TARGET = lab1_prog
PROG_FILES_TO_LINK = main.o io/io.o cs/cs.o

$(PROG_TARGET): $(PROG_FILES_TO_LINK)
	$(CXXPROC) $(CPPFLAGS) -o $(PROG_TARGET) $(PROG_FILES_TO_LINK)

%.o : %.cpp 
	$(CXXPROC) $(CPPFLAGS) -o $*.o -c $*.cpp

clean:
	rm -f $(PROG_TARGET)
	rm -f */*.o *.o