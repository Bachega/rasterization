#include <cstring>
#include <iostream>
#include <cstdlib>
#include <SDL2/SDL.h>
#include <stdio.h>

#include "polygon.hpp"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define HASH(x)(x%768)

struct bucket
{
    int y_max;
    int x_min;
    int p1_x;
    int p1_y;
    int p2_x;
    int p2_y;
    int numerator;
    int denomenator;
    int increment;
    struct bucket * next;
};

typedef struct edgetable
{
    struct bucket ** table;
    unsigned int max_y;
    unsigned int min_y;
}ET;

void clearWindow(SDL_Surface * surface);
void putPixel(SDL_Surface * surface, int x, int y);
void incLine(SDL_Surface * surface, int x1, int y1, int x2, int y2);
void raster(SDL_Surface * surface, int x1, int y1, int x2, int y2);
void drawLine(SDL_Surface * surface, int x1, int y1, int x2, int y2);
void drawCrosses(SDL_Surface * surface);
void paintScreen(SDL_Surface * surface);
void drawCircle(SDL_Surface * surface, int x_origin, int y_origin, int r);
void drawPoly(SDL_Surface * surface, Polygon * p);
void insertSortedAET(struct bucket ** aet, struct bucket * edge);
void removeByMaxY(struct bucket ** aet, int y_max);
void fillPoly(SDL_Surface * surface, Polygon * pol, SDL_Window * window);
ET * createEdgeTable();
void insertSortedET(Point * p, Point * p_head, ET * edge_table);
void buildEdgeTable(Polygon * pol, ET * edge_table);
void printET(ET * edge_table);
void ET_test();
void sort(struct bucket ** aet);
struct bucket * merge(struct bucket * a, struct bucket * b);
void printAET(struct bucket * aet);