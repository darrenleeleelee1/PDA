CXXPROC = g++
CPPFLAGS = -g -O0 -std=c++17
INCLUDE = -I ./inc

FM_TO_LINK = src/fm/fm.o src/fm/fm_main.o
DS_TO_LINK = src/ds/layout.o
LG_TO_LINK = src/lg/lg.o src/lg/lg_main.o
INPUT_TO_LINK = src/io/input.o

PROG_FILES_TO_LINK = main.o $(INPUT_TO_LINK) $(DS_TO_LINK) $(FM_TO_LINK) $(LG_TO_LINK)
PROG_TARGET = Lab3

$(PROG_TARGET): $(PROG_FILES_TO_LINK)
	$(CXXPROC) $(CPPFLAGS) $(INCLUDE) -o $(PROG_TARGET) $^

%.o : %.cpp
	$(CXXPROC) $(CPPFLAGS) $(INCLUDE) -o $@ -c $^

clean:
	rm -f $(PROG_TARGET)
	rm -f */*/*.o */*.o *.o