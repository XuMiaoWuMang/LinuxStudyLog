BIN = error
SRC = Error.cc
OBJ = $(SRC:.cc=.o)

$(BIN): $(OBJ)
	g++ -o $(BIN) $(OBJ) --std=c++11

$(OBJ): $(SRC)
	g++ -g -c $(SRC) --std=c++11

.PHONY: clean
clean:
	rm -f $(OBJ) $(BIN)
