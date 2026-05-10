#include <iostream>
#include <string>
#include "EmulatorChip-8/Emulator.h"
#include "EmulatorChip-8/EmulationLoop.h"
#include "desassembler/desassembler.h"
#include "debug/DebugMode.h"

int main() {
    std::cout << "======================================================" << std::endl;
    std::cout << "              CHIP-8 EMULATOR v1.0                    " << std::endl;
    std::cout << "======================================================" << std::endl;
    std::cout << " [1] Start Emulator (Normal Mode)" << std::endl;
    std::cout << " [2] Start Emulator (Debugger / Game Mode)" << std::endl;
    std::cout << " [3] Disassembler (static)" << std::endl;
    std::cout << " [4] Show Keybindings & Readme" << std::endl;
    std::cout << " [5] Exit" << std::endl;
    std::cout << "------------------------------------------------------" << std::endl;
    std::cout << " Selection > ";

    int selection;
    std::cin >> selection;

    if (selection == 1) {
        std::string ROM_choice;
        std::cout << "Select your ROM path to load:" << std::endl;
        std::cin.ignore();
        std::getline(std::cin, ROM_choice);

        chip8 cpu;
        chip8_init(&cpu);
        if (!chip8_load_rom(&cpu, ROM_choice.c_str())) {
            std::cerr << "Failed to load ROM!" << std::endl;
            return 1;
        }

        run_emulation(&cpu);

    }
    else if (selection == 3) {
        std::string ROM_choice;
        std::cout << "Select your ROM path to load:" << std::endl;
        std::cin.ignore();
        std::getline(std::cin, ROM_choice);
        disassemble(ROM_choice.c_str());
    }
    else if (selection == 2) {
        std::string ROM_choice;
        std::cout << "Select your ROM path to load (Debug Mode):" << std::endl;
        std::cin.ignore();
        std::getline(std::cin, ROM_choice);

        chip8 cpu;
        chip8_init(&cpu);
        if (!chip8_load_rom(&cpu, ROM_choice.c_str())) {
            std::cerr << "Failed to load ROM!" << std::endl;
            return 1;
        }

        run_debugger(&cpu);
    }
    else if (selection == 4) {

        std::cout << "Keybindings & README : ..." << std::endl;
    }
    else if (selection == 5) {
        std::cout << "Exiting. Goodbye!" << std::endl;
        return 0;
    }
    else {
        std::cout << "Invalid selection." << std::endl;
    }

    return 0;
}

       

