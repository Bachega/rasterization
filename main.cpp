#include "raster.hpp"

int main(int argc, char ** argv)
{
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
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
                running = false;
            else if(event.button.button == SDL_BUTTON_LEFT)
            {
                // ET_test();
                Polygon * p1 = createPoly();
                Polygon * p2 = createPoly();
                // insertPoint(p1, 20, 30);
                // insertPoint(p1, 70, 10);
                // insertPoint(p1, 130, 50);
                // insertPoint(p1, 130, 110);
                // insertPoint(p1, 70, 70);
                // insertPoint(p1, 20, 90);
                // fillPoly(surface, p1, window);

                // insertPoint(p2, 20, 30 + 200);
                // insertPoint(p2, 70, 10 + 200);
                // insertPoint(p2, 130, 50 + 200);
                // insertPoint(p2, 130, 110 + 200);
                // insertPoint(p2, 70, 70 + 200);
                // insertPoint(p2, 20, 90 + 200);
                // drawPoly(surface, p2);
                insertPoint(p1, 20, 40); // A
                insertPoint(p1, 30, 30); // B
                insertPoint(p1, 20, 20); // C
                insertPoint(p1, 50, 20); // D
                insertPoint(p1, 40, 30); // E
                insertPoint(p1, 50, 40); // F
                insertPoint(p1, 40, 50); // G
                insertPoint(p1, 40, 40); // H
                fillPoly(surface, p1, window);

                // insertPoint(p2, 20, 30 + 200);
                // insertPoint(p2, 70, 10 + 200);
                // insertPoint(p2, 130, 50 + 200);
                // insertPoint(p2, 130, 110 + 200);
                // insertPoint(p2, 70, 70 + 200);
                // insertPoint(p2, 20, 90 + 200);
                // drawPoly(surface, p2);
            }
        }
        SDL_UpdateWindowSurface(window);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
