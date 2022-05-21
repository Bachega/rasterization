using namespace std;

#include <iostream>
#include <cstring>
#include <SDL2/SDL.h>

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

void clearWindow(SDL_Surface * surface) {
    SDL_LockSurface(surface);
    SDL_memset(surface->pixels, 255, surface->h * surface->pitch);
    SDL_UnlockSurface(surface);
}

void putPixel(SDL_Surface * surface, int x, int y) {
    int offset = y * surface->pitch + x * surface->format->BytesPerPixel;
    Uint8 * pixels = (Uint8*)surface->pixels;
    pixels[offset] = 0;
    pixels[offset + 1] = 0;
    pixels[offset + 2] = 0;
}

void swap(int * x1, int * y1, int * x2, int * y2) {
    int tempX, tempY;
    tempX = *x1;
    tempY = *y1;
    *x1 = *x2;
    *y1 = *y2;
    *x2 = tempX;
    *y2 = tempY;
}

void incLine(SDL_Surface * surface, int x1, int y1, int x2, int y2) {
    int dx, dy, incE, incNE, d, x, y, y_offset = 1;

    if(x1 > x2)
        swap(&x1, &y1, &x2, &y2);
    
    dx = x2 - x1;
    dy = y2 - y1;

    if(dy < 0) {
        dy = -dy;
        y_offset = -1;
    }

    d = 2 * dy - dx;
    incE = 2 * dy;
    incNE = 2 * (dy - dx);
    x = x1;
    y = y1;

    putPixel(surface, x, y);
    while(x < x2) {
        if(d <= 0) {
            d += incE;
            x++;
        } else {
            d += incNE;
            x++;
            y += y_offset;
        }
        putPixel(surface, x, y);
    }
}

void drawLine(SDL_Surface * surface, int x1, int y1, int x2, int y2) {
    SDL_LockSurface(surface);
    incLine(surface, x1, y1, x2, y2);
    SDL_UnlockSurface(surface);
}

int main(int argc, char ** argv) {
    SDL_Window * window = nullptr;
    SDL_Surface * surface = nullptr;

    if(SDL_Init(SDL_INIT_VIDEO) < 0)
        std::cout << SDL_GetError() << endl;

    window = SDL_CreateWindow("Rasterization",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED_MASK,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN);
    surface = SDL_GetWindowSurface(window);

    clearWindow(surface);

    bool running = true;
    SDL_Event event;
    while(running) {
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT)
                running = false;
            else if(event.button.button == SDL_BUTTON_LEFT) {
                // drawLine(surface, 0, 0, 256, 256);
                // drawLine(surface, 256, 256, 0, 0);
                // drawLine(surface, 0, 512, 256, 0);
                drawLine(surface, 0, 0, SCREEN_WIDTH-1, SCREEN_HEIGHT-1);
                drawLine(surface, 0, SCREEN_HEIGHT-1, SCREEN_WIDTH-1, 0);
                // drawLine(surface, SCREEN_WIDTH-1, SCREEN_HEIGHT-1, 0, 0);
            }
        }
        SDL_UpdateWindowSurface(window);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
