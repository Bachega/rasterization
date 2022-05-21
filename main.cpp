#include "raster.hpp"

int main(int argc, char ** argv) {
    SDL_Window * window = nullptr;
    SDL_Surface * surface = nullptr;

    if(SDL_Init(SDL_INIT_VIDEO) < 0)
        std::cout << SDL_GetError() << std::endl;

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
