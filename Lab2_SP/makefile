CXXPROC = g++
CPPFLAGS = -g -O0 -std=c++17
INCLUDE = -I ./inc

PROG_FILES_TO_LINK = main.o src/io/input.o src/ds/layout.o
PROG_TARGET = Lab2

$(PROG_TARGET): $(PROG_FILES_TO_LINK)
	$(CXXPROC) $(CPPFLAGS) $(INCLUDE) -o $(PROG_TARGET) $^

%.o : %.cpp 
	$(CXXPROC) $(CPPFLAGS) $(INCLUDE) -o $@ -c $^

clean:
	rm -f $(PROG_TARGET)
	rm -f */*/*.o */*.o *.o