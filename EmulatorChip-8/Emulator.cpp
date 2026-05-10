#include "Emulator.h"
#include <fstream>
#include <iostream>
#include <cstring>
#include <cstdlib>

void chip8_init(chip8* cpu) {
    std::memset(cpu, 0, sizeof(chip8));
    cpu->pc = 0x200;
    cpu->timer_counter = 0;

    uint8_t fontset[80] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0,
        0x20, 0x60, 0x20, 0x20, 0x70,
        0xF0, 0x10, 0xF0, 0x80, 0xF0,
        0xF0, 0x10, 0xF0, 0x10, 0xF0,
        0x90, 0x90, 0xF0, 0x10, 0x10,
        0xF0, 0x80, 0xF0, 0x10, 0xF0,
        0xF0, 0x80, 0xF0, 0x90, 0xF0,
        0xF0, 0x10, 0x20, 0x40, 0x40,
        0xF0, 0x90, 0xF0, 0x90, 0xF0,
        0xF0, 0x90, 0xF0, 0x10, 0xF0,
        0xF0, 0x90, 0xF0, 0x90, 0x90,
        0xE0, 0x90, 0xE0, 0x90, 0xE0,
        0xF0, 0x80, 0x80, 0x80, 0xF0,
        0xE0, 0x90, 0x90, 0x90, 0xE0,
        0xF0, 0x80, 0xF0, 0x80, 0xF0,
        0xF0, 0x80, 0xF0, 0x80, 0x80
    };

    std::memcpy(&cpu->memory[0], fontset, sizeof(fontset));
}

void chip8_update_timers(chip8* cpu) {
    cpu->timer_counter++;
    if (cpu->timer_counter >= 8) {
        cpu->timer_counter = 0;
        if (cpu->delay_timer > 0) cpu->delay_timer--;
        if (cpu->sound_timer > 0) cpu->sound_timer--;
    }
}

bool chip8_load_rom(chip8* cpu, const char* filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) return false;

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    if (size > (4096 - 0x200)) return false;

    file.read(reinterpret_cast<char*>(&cpu->memory[0x200]), size);
    return true;
}

void chip8_step(chip8* cpu) {
    uint16_t inst = (cpu->memory[cpu->pc] << 8) | cpu->memory[cpu->pc + 1];
    cpu->pc += 2;

    uint8_t op = (inst & 0xF000) >> 12;
    uint8_t x = (inst & 0x0F00) >> 8;
    uint8_t y = (inst & 0x00F0) >> 4;
    uint8_t nn = (inst & 0x00FF);
    uint16_t nnn = (inst & 0x0FFF);

    switch (op) {
        case 0x0:
            if (nn == 0xE0) {
                std::memset(cpu->display, 0, sizeof(cpu->display));
            } else if (nn == 0xEE) {
                if (cpu->sp > 0) {
                    cpu->sp--;
                    cpu->pc = cpu->stack[cpu->sp];
                }
            }
            break;

        case 0x1: cpu->pc = nnn; break;
        case 0x2:
            if (cpu->sp >= 15) break;
            cpu->stack[cpu->sp] = cpu->pc;
            cpu->sp++;
            cpu->pc = nnn;
            break;

        case 0x3: if (cpu->V[x] == nn) cpu->pc += 2; break;
        case 0x4: if (cpu->V[x] != nn) cpu->pc += 2; break;
        case 0x5: if (cpu->V[x] == cpu->V[y]) cpu->pc += 2; break;
        case 0x6: cpu->V[x] = nn; break;
        case 0x7: cpu->V[x] += nn; break;

        case 0x8:
            switch (inst & 0x000F) {
                case 0x0: cpu->V[x] = cpu->V[y]; break;
                case 0x1: cpu->V[x] |= cpu->V[y]; break;
                case 0x2: cpu->V[x] &= cpu->V[y]; break;
                case 0x3: cpu->V[x] ^= cpu->V[y]; break;
                case 0x4: {
                    uint16_t sum = cpu->V[x] + cpu->V[y];
                    cpu->V[0xF] = (sum > 0xFF) ? 1 : 0;
                    cpu->V[x] = sum & 0xFF;
                    break;
                }
                case 0x5: {
                    cpu->V[0xF] = (cpu->V[x] >= cpu->V[y]) ? 1 : 0;
                    cpu->V[x] -= cpu->V[y];
                    break;
                }
                case 0x6: {
                    cpu->V[0xF] = cpu->V[x] & 0x1;
                    cpu->V[x] >>= 1;
                    break;
                }
                case 0x7: {
                    cpu->V[0xF] = (cpu->V[y] >= cpu->V[x]) ? 1 : 0;
                    cpu->V[x] = cpu->V[y] - cpu->V[x];
                    break;
                }
                case 0xE: {
                    cpu->V[0xF] = (cpu->V[x] & 0x80) ? 1 : 0;
                    cpu->V[x] <<= 1;
                    break;
                }
            }
            break;

        case 0x9: if (cpu->V[x] != cpu->V[y]) cpu->pc += 2; break;
        case 0xA: cpu->I = nnn; break;
        case 0xB: cpu->pc = nnn + cpu->V[0]; break;
        case 0xC: cpu->V[x] = (rand() % 256) & nn; break;

        case 0xD: {
            uint8_t x_pos = cpu->V[x] % 64;
            uint8_t y_pos = cpu->V[y] % 32;
            cpu->V[0xF] = 0;

            for (int row = 0; row < nn && y_pos + row < 32; row++) {
                uint8_t sprite_byte = cpu->memory[cpu->I + row];
                for (int col = 0; col < 8 && x_pos + col < 64; col++) {
                    if (sprite_byte & (0x80 >> col)) {
                        uint16_t pixel_index = (y_pos + row) * 64 + (x_pos + col);
                        if (cpu->display[pixel_index] == 1) {
                            cpu->V[0xF] = 1;
                        }
                        cpu->display[pixel_index] ^= 1;
                    }
                }
            }
            break;
        }

        case 0xE:
            if (nn == 0x9E) {
                if (cpu->keypad[cpu->V[x]]) cpu->pc += 2;
            } else if (nn == 0xA1) {
                if (!cpu->keypad[cpu->V[x]]) cpu->pc += 2;
            }
            break;

        case 0xF:
            switch (nn) {
                case 0x07: cpu->V[x] = cpu->delay_timer; break;
                case 0x0A: {
                    bool key_pressed = false;
                    for (int i = 0; i < 16; i++) {
                        if (cpu->keypad[i]) {
                            cpu->V[x] = i;
                            key_pressed = true;
                            break;
                        }
                    }
                    if (!key_pressed) cpu->pc -= 2;
                    break;
                }
                case 0x15: cpu->delay_timer = cpu->V[x]; break;
                case 0x18: cpu->sound_timer = cpu->V[x]; break;
                case 0x1E: cpu->I += cpu->V[x]; break;
                case 0x29: cpu->I = cpu->V[x] * 5; break;
                case 0x33: {
                    cpu->memory[cpu->I] = cpu->V[x] / 100;
                    cpu->memory[cpu->I + 1] = (cpu->V[x] / 10) % 10;
                    cpu->memory[cpu->I + 2] = cpu->V[x] % 10;
                    break;
                }
                case 0x55:
                    for (int i = 0; i <= x && cpu->I + i < 4096; i++) {
                        cpu->memory[cpu->I + i] = cpu->V[i];
                    }
                    break;
                case 0x65:
                    for (int i = 0; i <= x && cpu->I + i < 4096; i++) {
                        cpu->V[i] = cpu->memory[cpu->I + i];
                    }
                    break;
            }
            break;
    }
}