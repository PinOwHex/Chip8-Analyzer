#include <iostream>
#include <fstream>
#include <cstdint>
#include "EmulatorChip-8/Emulator.h"

void disassemble(const char* filename) {
    uint8_t buffer[4096];
    std::ifstream file(filename, std::ios::binary);
    file.read((char*)buffer, 4096);

    for (int i = 0; i < file.gcount(); i += 2) {
        uint16_t instruction = (buffer[i] << 8) | buffer[i + 1];
        uint8_t opcode = (instruction & 0xF000) >> 12;
        uint8_t x = (instruction & 0x0F00) >> 8;
        uint8_t y = (instruction & 0x00F0) >> 4;
        uint8_t n = (instruction & 0x000F);
        uint8_t nn = (instruction & 0x00FF);
        uint16_t nnn = (instruction & 0x0FFF);

        switch (opcode) {
            case 0x0:
                if (nn == 0xE0) std::cout << "CLS" << std::endl;
                else if (nn == 0xEE) std::cout << "RET" << std::endl;
                else std::cout << "SYS 0x" << std::hex << nnn << std::endl;
                break;

            case 0x1: std::cout << "JP 0x" << std::hex << nnn << std::endl; break;
            case 0x2: std::cout << "CALL 0x" << std::hex << nnn << std::endl; break;
            case 0x3: std::cout << "SE V" << std::hex << (int)x << ", 0x" << (int)nn << std::endl; break;
            case 0x4: std::cout << "SNE V" << std::hex << (int)x << ", 0x" << (int)nn << std::endl; break;
            case 0x5: std::cout << "SE V" << std::hex << (int)x << ", V" << (int)y << std::endl; break;
            case 0x6: std::cout << "LD V" << std::hex << (int)x << ", 0x" << (int)nn << std::endl; break;
            case 0x7: std::cout << "ADD V" << std::hex << (int)x << ", 0x" << (int)nn << std::endl; break;

            case 0x8:
                switch (n) {
                    case 0x0: std::cout << "LD V" << std::hex << (int)x << ", V" << (int)y << std::endl; break;
                    case 0x1: std::cout << "OR V" << std::hex << (int)x << ", V" << (int)y << std::endl; break;
                    case 0x2: std::cout << "AND V" << std::hex << (int)x << ", V" << (int)y << std::endl; break;
                    case 0x3: std::cout << "XOR V" << std::hex << (int)x << ", V" << (int)y << std::endl; break;
                    case 0x4: std::cout << "ADD V" << std::hex << (int)x << ", V" << (int)y << std::endl; break;
                    case 0x5: std::cout << "SUB V" << std::hex << (int)x << ", V" << (int)y << std::endl; break;
                    case 0x6: std::cout << "SHR V" << std::hex << (int)x << std::endl; break;
                    case 0x7: std::cout << "SUBN V" << std::hex << (int)x << ", V" << (int)y << std::endl; break;
                    case 0xE: std::cout << "SHL V" << std::hex << (int)x << std::endl; break;
                }
                break;

            case 0x9: std::cout << "SNE V" << std::hex << (int)x << ", V" << (int)y << std::endl; break;
            case 0xA: std::cout << "LD I, 0x" << std::hex << nnn << std::endl; break;
            case 0xB: std::cout << "JP V0, 0x" << std::hex << nnn << std::endl; break;
            case 0xC: std::cout << "RND V" << std::hex << (int)x << ", 0x" << (int)nn << std::endl; break;
            case 0xD: std::cout << "DRW V" << std::hex << (int)x << ", V" << (int)y << ", " << (int)n << std::endl; break;

            case 0xE:
                if (nn == 0x9E) std::cout << "SKP V" << std::hex << (int)x << std::endl;
                else if (nn == 0xA1) std::cout << "SKNP V" << std::hex << (int)x << std::endl;
                break;

            case 0xF:
                switch (nn) {
                    case 0x07: std::cout << "LD V" << std::hex << (int)x << ", DT" << std::endl; break;
                    case 0x0A: std::cout << "LD V" << std::hex << (int)x << ", K" << std::endl; break;
                    case 0x15: std::cout << "LD DT, V" << std::hex << (int)x << std::endl; break;
                    case 0x18: std::cout << "LD ST, V" << std::hex << (int)x << std::endl; break;
                    case 0x1E: std::cout << "ADD I, V" << std::hex << (int)x << std::endl; break;
                    case 0x29: std::cout << "LD F, V" << std::hex << (int)x << std::endl; break;
                    case 0x33: std::cout << "LD B, V" << std::hex << (int)x << std::endl; break;
                    case 0x55: std::cout << "LD [I], V" << std::hex << (int)x << std::endl; break;
                    case 0x65: std::cout << "LD V" << std::hex << (int)x << ", [I]" << std::endl; break;
                }
                break;

            default:
                std::cout << "Unknown opcode: 0x" << std::hex << instruction << std::endl;
                break;
        }

        if (instruction == 0x0000) {
            std::cout << "Empty memory." << std::endl;
            break;
        }
    }
}