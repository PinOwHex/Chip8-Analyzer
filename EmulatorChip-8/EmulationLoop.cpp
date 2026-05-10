#include "EmulationLoop.h"
#include <SDL2/SDL.h>
#include <iostream>

bool run_emulation(chip8* cpu) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL init failed: " << SDL_GetError() << std::endl;
        return false;
    }

    const int SCALE = 10;
    const int WIDTH = 64 * SCALE;
    const int HEIGHT = 32 * SCALE;

    SDL_Window* window = SDL_CreateWindow(
        "CHIP-8 Emulator",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH, HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_1: cpu->keypad[0x1] = 1; break;
                    case SDLK_2: cpu->keypad[0x2] = 1; break;
                    case SDLK_3: cpu->keypad[0x3] = 1; break;
                    case SDLK_4: cpu->keypad[0xC] = 1; break;
                    case SDLK_q: cpu->keypad[0x4] = 1; break;
                    case SDLK_w: cpu->keypad[0x5] = 1; break;
                    case SDLK_e: cpu->keypad[0x6] = 1; break;
                    case SDLK_r: cpu->keypad[0xD] = 1; break;
                    case SDLK_a: cpu->keypad[0x7] = 1; break;
                    case SDLK_s: cpu->keypad[0x8] = 1; break;
                    case SDLK_d: cpu->keypad[0x9] = 1; break;
                    case SDLK_f: cpu->keypad[0xE] = 1; break;
                    case SDLK_z: cpu->keypad[0xA] = 1; break;
                    case SDLK_x: cpu->keypad[0x0] = 1; break;
                    case SDLK_c: cpu->keypad[0xB] = 1; break;
                    case SDLK_v: cpu->keypad[0xF] = 1; break;
                    case SDLK_ESCAPE: running = false; break;
                }
            }
            if (event.type == SDL_KEYUP) {
                switch (event.key.keysym.sym) {
                    case SDLK_1: cpu->keypad[0x1] = 0; break;
                    case SDLK_2: cpu->keypad[0x2] = 0; break;
                    case SDLK_3: cpu->keypad[0x3] = 0; break;
                    case SDLK_4: cpu->keypad[0xC] = 0; break;
                    case SDLK_q: cpu->keypad[0x4] = 0; break;
                    case SDLK_w: cpu->keypad[0x5] = 0; break;
                    case SDLK_e: cpu->keypad[0x6] = 0; break;
                    case SDLK_r: cpu->keypad[0xD] = 0; break;
                    case SDLK_a: cpu->keypad[0x7] = 0; break;
                    case SDLK_s: cpu->keypad[0x8] = 0; break;
                    case SDLK_d: cpu->keypad[0x9] = 0; break;
                    case SDLK_f: cpu->keypad[0xE] = 0; break;
                    case SDLK_z: cpu->keypad[0xA] = 0; break;
                    case SDLK_x: cpu->keypad[0x0] = 0; break;
                    case SDLK_c: cpu->keypad[0xB] = 0; break;
                    case SDLK_v: cpu->keypad[0xF] = 0; break;
                }
            }
        }

        chip8_step(cpu);
        chip8_update_timers(cpu);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        for (int y = 0; y < 32; y++) {
            for (int x = 0; x < 64; x++) {
                if (cpu->display[y * 64 + x] != 0) {
                    SDL_Rect pixel_rect = {
                        x * SCALE, y * SCALE,
                        SCALE, SCALE
                    };
                    SDL_RenderFillRect(renderer, &pixel_rect);
                }
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(2);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return true;
}