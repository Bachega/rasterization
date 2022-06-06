#include <stdlib.h>

typedef struct point
{
    int x;
    int y;
    struct point * next;
}Point;

typedef struct polygon
{
    struct point * head;
    struct point * foot;
    int n;
}Polygon;

Point * createPoint(int x, int y);
Polygon * createPoly();
int insertPoint(Polygon * p, int x, int y);