GXX = g++
FLAGS = -std=c++20 -Wall -Wextra -pedantic
BIN = grep

$(BIN): main.cpp
	$(GXX) $< -o $@ $(FLAGS)

clean:
	del .\$(BIN).exe