CC=g++
BIN=bin
LIBS=


INCLUDE=-I src include

CFLAGS=-fopenmp -O3 -c -Wall -std=c++14 $(INCLUDE)
LDFLAGS=-fopenmp -std=c++14 #-lprofiler -Wl,-no_pie
SOURCES= board.cpp othello.cpp player.cpp game.cpp

OBJECTS=$(addprefix $(BIN)/, $(SOURCES:.cpp=.o))

INCS := $(shell find -type f -name "*.hpp")



EXECUTABLE=$(BIN)/main
vpath %.cpp src

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) $(LIBS) -o $@

-include $(BIN)/$(OBJECTS:.o=.d)

$(BIN)/%.o: %.cpp $(INCS)
	$(CC) -c $(CFLAGS) $< -o $(BIN)/$*.o
	$(CC) -MM $(CFLAGS) $< > $(BIN)/$*.d
	@mv -f $(BIN)/$*.d $(BIN)/$*.d.tmp
	@sed -e 's|.*:|$(BIN)/$*.o:|' < $(BIN)/$*.d.tmp > $(BIN)/$*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $(BIN)/$*.d.tmp | fmt -1 | \
	sed -e 's/^ *//' -e 's/$$/:/' >> $(BIN)/$*.d
	@rm -f $(BIN)/$*.d.tmp


clean:
	rm -f $(BIN)/*.o $(BIN)/*.d $(EXECUTABLE)