#include <cstring>
#include <iostream>
#include <cstdlib>
#include <SDL2/SDL.h>
#include <stdio.h>

#include "polygon.hpp"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define HASH(x)(x%768)

typedef struct line_list
{
    double x1;
    double y1;
    double x2;
    double y2;
    struct line_list * next;
}LineList;

typedef struct clipping_rec
{
    double x_min;
    double y_min;
    double x_max;
    double y_max;
}ClipRec;

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

const int INSIDE = 0; // 0000
const int LEFT = 1; // 0001
const int RIGHT = 2; // 0010
const int BOTTOM = 4; // 0100
const int TOP = 8; // 1000

void clearWindow(SDL_Surface * surface);
void putPixel(SDL_Surface * surface, int x, int y);
void incLine(SDL_Surface * surface, int x1, int y1, int x2, int y2, int dx, int dy, bool slope, bool simetric);
void raster(SDL_Surface * surface, int x1, int y1, int x2, int y2);
void drawLine(SDL_Surface * surface, int x1, int y1, int x2, int y2);
void drawCrosses(SDL_Surface * surface);
void paintScreen(SDL_Surface * surface);
void drawCircle(SDL_Surface * surface, int x_origin, int y_origin, int r);
void drawPoly(SDL_Surface * surface, Polygon * pol);
void insertLine(LineList ** list, double x1, double y1, double x2, double y2);
void drawLineList(SDL_Surface * surface, LineList * l);
ClipRec * createClipRec(double x_min, double y_min, double x_max, double y_max);
int generateCode(double x, double y, ClipRec * rec);
void cohenSutherland(SDL_Surface * surface, ClipRec * rec, LineList * l);
void insertSortedAET(struct bucket ** aet, struct bucket * edge);
void removeByMaxY(struct bucket ** aet, int y_max);
void printAET(struct bucket * aet);
void fillPoly(SDL_Surface * surface, Polygon * pol);
ET * createEdgeTable();
void insertSortedET(Point * p, Point * p_head, ET * edge_table);
void buildEdgeTable(Polygon * pol, ET * edge_table);
void printET(ET * edge_table);
void ET_test();
void sort(struct bucket ** aet);
struct bucket * merge(struct bucket * a, struct bucket * b);