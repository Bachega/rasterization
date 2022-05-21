#include <iostream>
#include <cstring>
#include <cstdlib>
#include <SDL2/SDL.h>

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

void clearWindow(SDL_Surface * surface);
void putPixel(SDL_Surface * surface, int x, int y);
void incLine(SDL_Surface * surface, int x1, int y1, int x2, int y2);
void raster(SDL_Surface * surface, int x1, int y1, int x2, int y2);
void drawLine(SDL_Surface * surface, int x1, int y1, int x2, int y2);
void drawCrosses(SDL_Surface * surface);