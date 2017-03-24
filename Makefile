CXX=g++
LDLIBS=-lm -lncurses
ifdef mk5
CPPFLAGS=-Dmk5
endif
DEPS= robby.h robot.h grid.h
OBJ = robby.o robot.o grid.o
binaries = robby
print-% : ; @echo $* = $($*)

robby: $(OBJ)
	$(CXX) -o $@ $^ $(LDLIBS)
clean:
	rm -f $(binaries) *.o
