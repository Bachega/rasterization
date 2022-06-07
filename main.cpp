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
    while(running)
    {
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
                running = false;
            else if(event.button.button == SDL_BUTTON_LEFT)
            {
                // Polygon * p1 = createPoly();
                // Polygon * p2 = createPoly();
                // insertPoint(p1, 200, 400); // A
                // insertPoint(p1, 300, 300); // B
                // insertPoint(p1, 200, 200); // C
                // insertPoint(p1, 500, 200); // D
                // insertPoint(p1, 400, 300); // E
                // insertPoint(p1, 500, 400); // F
                // insertPoint(p1, 400, 500); // G
                // insertPoint(p1, 400, 400); // H
                // fillPoly(surface, p1);

                // insertPoint(p2, 200 + 200, 400 + 200); // A
                // insertPoint(p2, 300 + 200, 300 + 200); // B
                // insertPoint(p2, 200 + 200, 200 + 200); // C
                // insertPoint(p2, 500 + 200, 200 + 200); // D
                // insertPoint(p2, 400 + 200, 300 + 200); // E
                // insertPoint(p2, 500 + 200, 400 + 200); // F
                // insertPoint(p2, 400 + 200, 500 + 200); // G
                // insertPoint(p2, 400 + 200, 400 + 200); // H
                // drawPoly(surface, p2);

                ClipRec * clipRec = createClipRec(200, 200, 400, 400);
                LineList * l = NULL;
                insertLine(&l, 50, 600, 600, 50);
                insertLine(&l, 50, 200, 200, 150);
                insertLine(&l, 500, 200, 600, 500);
                insertLine(&l, 10, 200, 100, 200);
                insertLine(&l, 50, 400, 600, 400);
                insertLine(&l, 400, 400, 100, 500);
                insertLine(&l, 295, 200, 600, 50);
                insertLine(&l, 463, 100, 300, 300);
                insertLine(&l, 204, 500, 700, 110);
                insertLine(&l, 300, 200, 300, 495);
                
                // drawLineList(surface, l);
                cohenSutherland(surface, clipRec, l);
            }
        }
        SDL_UpdateWindowSurface(window);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
