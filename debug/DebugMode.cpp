#include "DebugMode.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <SDL2/SDL.h>

void print_registers(const chip8* cpu) {
    std::cout << "=== REGISTERS ===" << std::endl;
    for (int i = 0; i < 16; i++) {
        std::cout << "V" << std::hex << i << ": 0x" << std::setw(2) << std::setfill('0') << (int)cpu->V[i];
        if (i % 4 == 3) std::cout << std::endl;
        else std::cout << "  ";
    }
    std::cout << "I: 0x" << std::hex << cpu->I << std::endl;
    std::cout << "PC: 0x" << std::hex << cpu->pc << std::endl;
    std::cout << "SP: " << std::dec << (int)cpu->sp << std::endl;
    std::cout << "DT: " << std::dec << (int)cpu->delay_timer << std::endl;
    std::cout << "ST: " << std::dec << (int)cpu->sound_timer << std::endl;
    std::cout << std::endl;
}

void print_memory(const chip8* cpu, uint16_t start, int lines = 8) {
    std::cout << "=== MEMORY (0x" << std::hex << start << ") ===" << std::endl;
    for (int line = 0; line < lines; line++) {
        uint16_t addr = start + line * 16;
        std::cout << std::hex << std::setw(4) << std::setfill('0') << addr << ": ";
        for (int i = 0; i < 16; i++) {
            if (addr + i < 4096) {
                std::cout << std::setw(2) << std::setfill('0') << (int)cpu->memory[addr + i] << " ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void print_display(const chip8* cpu) {
    std::cout << "=== DISPLAY (64x32) ===" << std::endl;
    for (int y = 0; y < 32; y++) {
        for (int x = 0; x < 64; x++) {
            std::cout << (cpu->display[y * 64 + x] ? "█" : " ");
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void print_stack(const chip8* cpu) {
    std::cout << "=== STACK ===" << std::endl;
    for (int i = 0; i < 16; i++) {
        if (i < cpu->sp) {
            std::cout << "[" << std::dec << i << "]: 0x" << std::hex << cpu->stack[i] << std::endl;
        } else {
            std::cout << "[" << std::dec << i << "]: ----" << std::endl;
        }
    }
    std::cout << std::endl;
}

void run_debugger(chip8* cpu) {
    std::string command;
    bool running = true;
    uint16_t breakpoint = 0xFFFF; // No breakpoint by default

    std::cout << "=== CHIP-8 DEBUGGER ===" << std::endl;
    std::cout << "Commands: step, continue, regs, mem [addr], display, stack, break [addr], quit" << std::endl;
    std::cout << std::endl;

    while (running) {
        std::cout << "DEBUG> ";
        std::getline(std::cin, command);

        if (command == "step" || command == "s") {
            uint16_t current_pc = cpu->pc;
            chip8_step(cpu);
            chip8_update_timers(cpu);

            uint16_t inst = (cpu->memory[current_pc] << 8) | cpu->memory[current_pc + 1];
            std::cout << "Executed: 0x" << std::hex << inst << " at 0x" << current_pc << std::endl;

        } else if (command == "continue" || command == "c") {
            std::cout << "Continuing execution... (Press Ctrl+C to stop)" << std::endl;

            // Simple continue - run until breakpoint or manual stop
            while (cpu->pc != breakpoint) {
                chip8_step(cpu);
                chip8_update_timers(cpu);

                // Check for key presses to interrupt
                SDL_Event event;
                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                        std::cout << "Execution interrupted at PC: 0x" << std::hex << cpu->pc << std::endl;
                        goto continue_done;
                    }
                }
                SDL_Delay(2); // Slow down execution for debugging
            }
            if (breakpoint != 0xFFFF) {
                std::cout << "Breakpoint hit at PC: 0x" << std::hex << cpu->pc << std::endl;
            }
            continue_done:;

        } else if (command == "regs" || command == "r") {
            print_registers(cpu);

        } else if (command.substr(0, 3) == "mem") {
            uint16_t addr = cpu->pc; // Default to current PC
            if (command.length() > 4) {
                try {
                    addr = std::stoi(command.substr(4), nullptr, 16);
                } catch (...) {
                    std::cout << "Invalid address format. Use hex: mem 200" << std::endl;
                    continue;
                }
            }
            print_memory(cpu, addr);

        } else if (command == "display" || command == "d") {
            print_display(cpu);

        } else if (command == "stack") {
            print_stack(cpu);

        } else if (command.substr(0, 5) == "break") {
            if (command.length() > 6) {
                try {
                    breakpoint = std::stoi(command.substr(6), nullptr, 16);
                    std::cout << "Breakpoint set at: 0x" << std::hex << breakpoint << std::endl;
                } catch (...) {
                    std::cout << "Invalid address format. Use hex: break 200" << std::endl;
                }
            } else {
                breakpoint = 0xFFFF;
                std::cout << "Breakpoint cleared" << std::endl;
            }

        } else if (command == "help" || command == "h") {
            std::cout << "Available commands:" << std::endl;
            std::cout << "  step (s)          - Execute one instruction" << std::endl;
            std::cout << "  continue (c)      - Run until breakpoint or interrupt" << std::endl;
            std::cout << "  regs (r)          - Show registers" << std::endl;
            std::cout << "  mem [addr]        - Show memory (default: current PC)" << std::endl;
            std::cout << "  display (d)       - Show display buffer" << std::endl;
            std::cout << "  stack             - Show stack contents" << std::endl;
            std::cout << "  break [addr]      - Set/clear breakpoint" << std::endl;
            std::cout << "  help (h)          - Show this help" << std::endl;
            std::cout << "  quit (q)          - Exit debugger" << std::endl;

        } else if (command == "quit" || command == "q") {
            running = false;

        } else if (!command.empty()) {
            std::cout << "Unknown command. Type 'help' for available commands." << std::endl;
        }
    }
}