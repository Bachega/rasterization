#include "raster.hpp"

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

void incLine(SDL_Surface * surface, int x1, int y1, int x2, int y2, int dx, int dy, bool slope, bool simetric) {
    int d = 2*dy - dx;
    int incE = 2*dy;
    int incNE = 2*(dy - dx);
    int x = x1;
    int y = y1;

    if(slope)
        putPixel(surface, y, (0 - x));
    else
        putPixel(surface, x, y);
    
    while(x < x2) {
        if(d <= 0) {
            d += incE;
            x++;
        } else {
            d += incNE;
            x++;
            if(simetric)
                y--;
            else
                y++;
        }
        if(slope)
            putPixel(surface, y, (0 - x));
        else
            putPixel(surface, x, y);
    }
}

void raster(SDL_Surface * surface, int x1, int y1, int x2, int y2) {
    bool slope = false, simetric = false;
    int temp, dx = x2 - x1, dy = y2 - y1;

    if((dy < 0 ? 0 - dy : dy) > (dx < 0 ? 0 - dx : dx)) {
        slope = true;
        temp = x1;
        x1 = 0 - y1;
        y1 = temp;
        temp = x2;
        x2 = 0 - y2;
        y2 = temp;
        dx = x2 - x1;
        dy = y2 - y1;
    }

    if(dx < 0) {
        temp = x1;
        x1 = x2;
        x2 = temp;
        temp = y1;
        y1 = y2;
        y2 - temp;
        dx = x2 - x1;
        dy = y2 - y1;
    }

    if(dy < 0) {
        dy = 0 - dy;
        simetric = true;
    }

    incLine(surface, x1, y1, x2, y2, dx, dy, slope, simetric);    
}

void drawLine(SDL_Surface * surface, int x1, int y1, int x2, int y2) {
    SDL_LockSurface(surface);
    raster(surface, x1, y1, x2, y2);
    SDL_UnlockSurface(surface);
}

void drawCrosses(SDL_Surface * surface) {
    SDL_LockSurface(surface);
    raster(surface, 0, 0, SCREEN_WIDTH-1, SCREEN_HEIGHT-1);

    raster(surface, 0, SCREEN_HEIGHT-1, SCREEN_WIDTH-1, 0);
    
    SDL_UnlockSurface(surface);
}