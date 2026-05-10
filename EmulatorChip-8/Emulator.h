
#pragma once

#include <cstdint>

struct chip8 {
    uint8_t memory[4096];
    uint8_t V[16];
    uint16_t I;
    uint16_t pc;
    uint16_t stack[16];
    uint8_t sp;
    uint8_t delay_timer;
    uint8_t sound_timer;
    uint32_t display[64 * 32];
    uint8_t keypad[16];
    uint32_t timer_counter;
};

void chip8_init(chip8* cpu);
bool chip8_load_rom(chip8* cpu, const char* filename);
void chip8_step(chip8* cpu);
void chip8_update_timers(chip8* cpu);
uint16_t chip8_fetch_instruction(const chip8* cpu);


void chip8_update_timers(chip8* cpu);

uint16_t chip8_fetch_instruction(const chip8* cpu);

