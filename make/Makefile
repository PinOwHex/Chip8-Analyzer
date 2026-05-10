# Makefile pour le projet desassembly

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I. -I./EmulatorChip-8 -I./desassembler -I./debug `pkg-config --cflags sdl2`
LDFLAGS =
LDLIBS = `pkg-config --libs sdl2`

SRC = main.cpp \
      EmulatorChip-8/Emulator.cpp \
      EmulatorChip-8/EmulationLoop.cpp \
      desassembler/desasembler.cpp \
      debug/DebugMode.cpp

OBJ = $(SRC:.cpp=.o)
TARGET = chip8

.PHONY: all run clean

all: $(TARGET)

run: $(TARGET)
	./$(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJ)
