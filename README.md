# CHIP-8 Emulator

A complete CHIP-8 emulator written in C++ with SDL2 graphics support.

## Features

- Full CHIP-8 instruction set implementation
- SDL2 graphics rendering (640x320 scaled display)
- Keyboard input mapping
- Sound and delay timers (60Hz)
- ROM loading and disassembly
- Stack overflow protection
- Memory bounds checking

## Requirements

- C++17 compiler (g++)
- SDL2 development libraries
- pkg-config

### Installation on Ubuntu/Debian

```bash
sudo apt update
sudo apt install libsdl2-dev pkg-config build-essential
```

## Building

```bash
# Clone or download the project
cd chip8-emulator

# Build using Makefile
make -f make/Makefile

# Or build manually
g++ -std=c++17 -Wall -Wextra -I. -I./EmulatorChip-8 -I./desassembler `pkg-config --cflags sdl2` \
    -o chip8 main.cpp EmulatorChip-8/Emulator.cpp EmulatorChip-8/EmulationLoop.cpp \
    desassembler/desasembler.cpp `pkg-config --libs sdl2`
```

## Usage

```bash
./chip8
```

### Menu Options

1. **Start Emulator**: Load and run a CHIP-8 ROM in normal mode
2. **Start Emulator (Debugger)**: Load and debug a CHIP-8 ROM with step-by-step execution
3. **Disassembler**: Static disassembly of ROM instructions
4. **Keybindings**: Show keyboard mapping
5. **Exit**: Quit the program

## Keyboard Mapping

CHIP-8 keypad maps to keyboard:

```
1 2 3 4  ->  1 2 3 C
Q W E R  ->  4 5 6 D
A S D F  ->  7 8 9 E
Z X C V  ->  A 0 B F
```

ESC to quit the emulator.

## Debugger

The emulator includes a built-in debugger for step-by-step execution and analysis:

### Debugger Commands

- `step` or `s` - Execute one instruction
- `continue` or `c` - Run until breakpoint (ESC to interrupt)
- `regs` or `r` - Display all registers (V0-VF, I, PC, SP, DT, ST)
- `mem [addr]` - Show memory contents (default: current PC)
- `display` or `d` - Show display buffer (64x32)
- `stack` - Show stack contents
- `break [addr]` - Set/clear breakpoint (hex address)
- `help` or `h` - Show available commands
- `quit` or `q` - Exit debugger

### Example Usage

```
DEBUG> regs
DEBUG> mem 200
DEBUG> step
DEBUG> display
DEBUG> break 250
DEBUG> continue
```

## Project Structure

```
├── main.cpp                    # Main menu and program entry
├── EmulatorChip-8/
│   ├── Emulator.h             # CHIP-8 struct definitions
│   ├── Emulator.cpp           # CPU emulation core
│   ├── EmulationLoop.h        # Graphics interface
│   └── EmulationLoop.cpp      # SDL2 window and event loop
├── debug/
│   ├── DebugMode.h            # Debugger interface
│   └── DebugMode.cpp          # Interactive debugger implementation
├── desassembler/
│   ├── desasembler.h          # Disassembler interface
│   └── desasembler.cpp        # Static ROM disassembly
└── make/
    └── Makefile               # Build configuration
```

## Technical Details

- **Memory**: 4KB RAM
- **Registers**: 16 general-purpose (V0-VF), I register
- **Display**: 64x32 monochrome pixels
- **Stack**: 16 levels
- **Timers**: Delay and sound timers at 60Hz
- **Clock**: ~500Hz instruction execution

## License

This project is open source. Feel free to use and modify.