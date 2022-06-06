#include "polygon.hpp"

Point * createPoint(int x, int y) 
{
    Point * p = (Point*)malloc(sizeof(Point));
    p->x = x;
    p->y = y;
    p->next = NULL;
    return p;
}

Polygon * createPoly()
{
    Polygon * p = (Polygon*)malloc(sizeof(Polygon));
    p->n = 0;
    return p;
}

int insertPoint(Polygon * p, int x, int y)
{  
    if(p->n == 0)
        p->head = p->foot = createPoint(x, y);
    else
        p->foot = p->foot->next = createPoint(x, y);
    p->n++;
    return 1;
}