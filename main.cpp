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

void incLine(SDL_Surface * surface, int x1, int y1, int x2, int y2) {
    int dx, dy, incE, incNE, d, x, y;
    
    dx = x2 - x1;
    dy = y2 - y1;
    d = 2 * dy - dx;
    incE = 2 * dy;
    incNE = 2 * (dy - dx);
    x = x1;
    y = y1;

    putPixel(surface, x, y);
    while(x < x2) {
        if(d <= 0) {
            // Choose E 
            d += incE;
            x++;
            // cout << "Chossing E\nGoing to (" << x << ", " << y << ")" << endl;
        } else {
            // Choose NE
            d += incNE;
            x++;
            y++;
            // cout << "Chossing NE\nGoing to (" << x << ", " << y << ")" << endl;
        }
        putPixel(surface, x, y);
    }
}

void drawLine(SDL_Surface * surface, int x1, int y1, int x2, int y2) {
    SDL_LockSurface(surface);
    incLine(surface, x1, (0 - y1), x2, (0 -y2));
    SDL_UnlockSurface(surface);
}

void drawSemiCircle(SDL_Surface * surface) {
    SDL_LockSurface(surface);
    // for(int y = 300; y <= 600; y++)
    //     incLine(surface, 300, 300, 600, y);

    for(int y = 600; y <= 900; y++)
        incLine(surface, 300, (0-300), y, (0-600));

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
                drawSemiCircle(surface);
            }
        }
        SDL_UpdateWindowSurface(window);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
