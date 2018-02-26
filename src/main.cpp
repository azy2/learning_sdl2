#include <stdio.h>
#include <stdlib.h>
#include <cstdint>
#include <vector>
#include <SDL2/SDL.h>

using namespace std;

static uint32_t WIDTH = 1280;
static uint32_t HEIGHT = 720;

class Pixel {
    public:
        uint8_t a;
        uint8_t r;
        uint8_t g;
        uint8_t b;
};

class Buffer {
    public:
        Buffer(uint32_t WIDTH, uint32_t HEIGHT) : w(WIDTH), h(HEIGHT) {
            pixels = new Pixel[w*h]();
        }

        ~Buffer() {
            delete [] pixels;
        }

        void fill(Pixel color) {
            Pixel* p = pixels;
            for(int y = 0; y < h; y++) {
                for (int x = 0; x < w; x++) {
                    *p = color;
                    p++;
                }
            }
        }

        void* getRawPixels() {
            return static_cast<void*>(pixels);
        }
    private:
        Pixel* pixels;
        uint32_t w;
        uint32_t h;
};

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("sdl", 0, 0, WIDTH, HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *ren = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (ren == NULL) {
        SDL_DestroyWindow(window);
        printf("SDL_CreateRenderer error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Texture* texture = SDL_CreateTexture(ren, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);

    Buffer buffer(WIDTH, HEIGHT);
    buffer.fill({0,0,0,255});

    bool quit = false;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_WINDOWEVENT) {
                if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
                    WIDTH = e.window.data1;
                    HEIGHT = e.window.data2;
                }
            }
        }

        SDL_UpdateTexture(texture, NULL, buffer.getRawPixels(), WIDTH*4);

        SDL_RenderClear(ren);
        SDL_RenderCopy(ren, texture, NULL, NULL);
        SDL_RenderPresent(ren);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
