BIN = sig
SRC = signal.cc
OBJ = $(SRC:.cc=.o)

$(BIN): $(OBJ)
	g++ -o $@ $^ --std=c++11
$(OBJ): $(SRC)
	g++ -c $< --std=c++11
	
.PHONY: clean
clean:
	rm -f $(OBJ) $(BIN)