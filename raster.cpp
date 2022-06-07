#include "raster.hpp"

void clearWindow(SDL_Surface * surface)
{
    SDL_LockSurface(surface);
    SDL_memset(surface->pixels, 255, surface->h * surface->pitch);
    SDL_UnlockSurface(surface);
}

void putPixel(SDL_Surface * surface, int x, int y)
{
    int offset = y * surface->pitch + x * surface->format->BytesPerPixel;
    Uint8 * pixels = (Uint8*)surface->pixels;
    pixels[offset] = 0;
    pixels[offset + 1] = 0;
    pixels[offset + 2] = 0;
}

void incLine(SDL_Surface * surface, int x1, int y1, int x2, int y2, int dx, int dy, bool slope, bool simetric)
{
    int d = 2*dy - dx;
    int incE = 2*dy;
    int incNE = 2*(dy - dx);
    int x = x1;
    int y = y1;

    if(slope)
        putPixel(surface, y, (0 - x));
    else
        putPixel(surface, x, y);
    
    while(x < x2)
    {
        if(d <= 0)
        {
            d += incE;
            x++;
        }
        else
        {
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

void raster(SDL_Surface * surface, int x1, int y1, int x2, int y2)
{
    bool slope = false, simetric = false;
    int temp, dx = x2 - x1, dy = y2 - y1;

    if((dy < 0 ? 0 - dy : dy) > (dx < 0 ? 0 - dx : dx))
    {
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

    if(dx < 0)
    {
        temp = x1;
        x1 = x2;
        x2 = temp;
        temp = y1;
        y1 = y2;
        y2 = temp;
        dx = x2 - x1;
        dy = y2 - y1;
    }

    if(dy < 0)
    {
        dy = 0 - dy;
        simetric = true;
    }

    incLine(surface, x1, y1, x2, y2, dx, dy, slope, simetric);    
}

void drawLine(SDL_Surface * surface, int x1, int y1, int x2, int y2)
{
    SDL_LockSurface(surface);
    raster(surface, x1, y1, x2, y2);
    SDL_UnlockSurface(surface);
}

void drawCrosses(SDL_Surface * surface)
{
    SDL_LockSurface(surface);
    raster(surface, (SCREEN_WIDTH)/2, 0, (SCREEN_WIDTH)/2, SCREEN_HEIGHT-1);
    raster(surface, 0, (SCREEN_HEIGHT)/2, SCREEN_WIDTH-1, (SCREEN_HEIGHT)/2);
    raster(surface, 0, 0, SCREEN_WIDTH-1, SCREEN_HEIGHT-1);
    raster(surface, 0, SCREEN_HEIGHT-1, SCREEN_WIDTH-1, 0);
    raster(surface, 0, 0, SCREEN_WIDTH-1, SCREEN_HEIGHT-1);
    SDL_UnlockSurface(surface);
}

void paintScreen(SDL_Surface * surface)
{
    SDL_LockSurface(surface);
    for(int x1 = 0, x2 = SCREEN_WIDTH - 1; x1 < SCREEN_WIDTH; x1++, x2--)
        raster(surface, x1, 0, x2, SCREEN_HEIGHT - 1);
    for(int y1 = 0, y2 = SCREEN_HEIGHT - 1; y1 < SCREEN_HEIGHT; y1++, y2--)
        raster(surface, 0, y1, SCREEN_WIDTH - 1, y2);
    SDL_UnlockSurface(surface);
}

void drawCircle(SDL_Surface * surface, int x_origin, int y_origin, int r)
{
    int x = 0, y = r, d = 1 - r;

    SDL_LockSurface(surface);
    putPixel(surface, x + x_origin, y + y_origin);
    putPixel(surface, x + x_origin, -y + y_origin);
    putPixel(surface, -x + x_origin, y + y_origin);
    putPixel(surface, -x + x_origin, -y + y_origin);
    putPixel(surface, y + x_origin, x + y_origin);
    putPixel(surface, y + x_origin, -x + y_origin);
    putPixel(surface, -y + x_origin, x + y_origin);
    putPixel(surface, -y + x_origin, -x + y_origin);
    while(y > x)
    {
        if(d < 0)
        {
            d = d + 2 * x + 3;
            x++;
        }
        else
        {
            d = d + 2 * (x - y) + 5;
            x++;
            y--;
        }
        putPixel(surface, x + x_origin, y + y_origin);
        putPixel(surface, x + x_origin, -y + y_origin);
        putPixel(surface, -x + x_origin, y + y_origin);
        putPixel(surface, -x + x_origin, -y + y_origin);
        putPixel(surface, y + x_origin, x + y_origin);
        putPixel(surface, y + x_origin, -x + y_origin);
        putPixel(surface, -y + x_origin, x + y_origin);
        putPixel(surface, -y + x_origin, -x + y_origin);
    }
    SDL_UnlockSurface(surface);
}

void drawPoly(SDL_Surface * surface, Polygon * pol)
{
    SDL_LockSurface(surface);
    Point * p = pol->head;
    while(p->next != NULL)
    {
        raster(surface, p->x, p->y, p->next->x, p->next->y);
        p = p->next;
    }
    raster(surface, pol->foot->x, pol->foot->y, pol->head->x, pol->head->y);
    SDL_UnlockSurface(surface);
}

void insertLine(LineList ** list, double x1, double y1, double x2, double y2)
{
    LineList * l = *list, * head = NULL;

    if(l == NULL)
    {
        l = (LineList*)malloc(sizeof(LineList));
        l->x1 = x1;
        l->y1 = y1;
        l->x2 = x2;
        l->y2 = y2;
        l->next = NULL;
        *list = l;
        return;
    }

    head = (LineList*)malloc(sizeof(LineList));
    head->x1 = x1;
    head->y1 = y1;
    head->x2 = x2;
    head->y2 = y2;
    head->next = l;
    *list = head;
}

void drawLineList(SDL_Surface * surface, LineList * l)
{   
    SDL_LockSurface(surface);
    while(l != NULL)
    {
        raster(surface, (int)l->x1, (int)l->y1, (int)l->x2, (int)l->y2);
        l = l->next;
    }
    SDL_UnlockSurface(surface);
}

ClipRec * createClipRec(double x_min, double y_min, double x_max, double y_max)
{
    ClipRec * rec = (ClipRec*)malloc(sizeof(ClipRec));
    rec->x_min = x_min;
    rec->y_min = y_min;
    rec->x_max = x_max;
    rec->y_max = y_max;
    return rec;
}

int generateCode(double x, double y, ClipRec * rec)
{
    int code = INSIDE;
    if(x < rec->x_min)
        code |= LEFT;
    else if(x > rec->x_max)
        code |= RIGHT;
    else if(y < rec->y_min)
        code |= BOTTOM;
    else if(y > rec->y_max)
        code |= TOP;
    return code;
}

void cohenSutherland(SDL_Surface * surface, ClipRec * rec, LineList * l)
{
    int code1, code2, outcode, accept;
    double x, y;
    LineList * line = l;

    SDL_LockSurface(surface);
    while(line)
    {
        code1 = generateCode(line->x1, line->y1, rec);
        code2 = generateCode(line->x2, line->y2, rec);
        accept = 0;
        for(;;)
        {
            if((code1 == 0) && (code2 == 0))
            {
                accept = 1;
                break;
            }
            else if(code1 & code2)
                break;
            else
            {
                if(code1 != 0)
                    outcode = code1;
                else
                    outcode = code2;

                if(outcode & TOP)
                {
                    x = line->x1 + (line->x2 - line->x1) * (rec->y_max - line->y1) / (line->y2 - line->y1);
                    y = rec->y_max;
                }
                else if(outcode & BOTTOM)
                {
                    x = line->x1 + (line->x2 - line->x1) * (rec->y_min - line->y1) / (line->y2 - line->y1);
                    y = rec->y_min;
                }
                else if(outcode & RIGHT)
                {
                    y = line->y1 + (line->y2 - line->y1) * (rec->x_max - line->x1) / (line->x2 - line->x1);
                    x = rec->x_max;
                }
                else if(outcode & LEFT)
                {
                    y = line->y1 + (line->y2 - line->y1) * (rec->x_min - line->x1) / (line->x2 - line->x1);
                    x = rec->x_min;
                }

                if(outcode == code1)
                {
                    line->x1 = x;
                    line->y1 = y;
                    code1 = generateCode(line->x1, line->y1, rec);
                }
                else
                {
                    line->x2 = x;
                    line->y2 = y;
                    code2 = generateCode(line->x2, line->y2, rec);
                }
            }
        }
        
        if(accept)
            raster(surface, (int)line->x1, (int)line->y1, (int)line->x2, (int)line->y2);
        
        line = line->next;
    }
    SDL_UnlockSurface(surface);
}

void insertSortedAET(struct bucket ** aet, struct bucket * edge)
{
    struct bucket * curr = *aet, * prev = NULL, * head = NULL;

    if(edge == NULL) return;    

    if(*aet == NULL)
    {
        *aet = edge;
        return;
    }
    
    prev = curr;
    while(edge != NULL && curr != NULL)
    {
        if(edge->x_min < curr->x_min)
        {
            if(prev == curr)
            {
                head = edge;
                edge = edge->next;
                head->next = curr;
                prev = head;
            }
            else
            {
                prev->next = edge;
                edge = edge->next;
                prev->next->next = curr;
                prev = prev->next;
            }
        }
        else
            curr = curr->next;
    }

    if(curr == NULL)
        while(edge != NULL)
        {
            prev->next = edge;
            prev = edge;
            edge = edge->next;
        }
    
    if(head != NULL)
        *aet = head;
}

void removeByMaxY(struct bucket ** aet, int y_max)
{
    struct bucket * curr = *aet, * prev = NULL, * head = NULL, * aux;
    prev = curr;
    while(curr != NULL)
    {
        if(curr->y_max == y_max)
        {
            if(prev == curr)
            {
                curr = curr->next;
                prev->next = NULL;
                free(prev);
                prev = curr;
                *aet = curr;
            }
            else
            {
                aux = curr;
                curr = curr->next;
                prev->next = curr;
                aux->next = NULL;
                free(aux);
            }
        }
        else
        {
            if(curr != NULL)
            {
                prev = curr;
                curr = curr->next;
            }
        }
    }
}

void printAET(struct bucket * aet)
{
    while(aet)
    {
        printf(" [%d, %d]{%d, %d} ", aet->y_max, aet->x_min, aet->numerator, aet->denomenator);
        aet = aet->next;
    }
}

void fillPoly(SDL_Surface * surface, Polygon * pol)
{
    int min_y, max_y, x1, x2, y_max1, y_max2, fill, count, sign;
    float num, den;
    ET * et = createEdgeTable();
    struct bucket * aet = NULL, * line = NULL;
    buildEdgeTable(pol, et);
    min_y = et->min_y;
    max_y = et->max_y;
  
    SDL_LockSurface(surface);
    for(int i = et->min_y; i < et->max_y; i++)
    {
        insertSortedAET(&aet, et->table[i]);
        et->table[i] = NULL;
        removeByMaxY(&aet, i);

        line = aet;
        x1 = x2 = y_max1 = y_max2 = count = fill = 0;
        while(line != NULL)
        {
            if(count%2 == 0)
            {
                x1 = line->x_min;
                y_max1 = line->y_max;
                if(x1 == x2)
                {
                    if (( (x1 == y_max1) && (x2 != y_max2)) || ( (x1 != y_max1) && (x2 == y_max2) ))
                    {
                        x2 = x1;
                        y_max2 = y_max1;
                    }
                    else
                        count++;
                }
                else
                    count++;
            }
            else
            {
                x2 = line->x_min;
                y_max2 = line->y_max;
                fill = 0;

                if(x1 == x2)
                {
                    if (( (x1 == y_max1) && (x2 != y_max2)) || ( (x1 != y_max1) && (x2 == y_max2) ))
                    {
                        x1 = x2;
                        y_max1 = y_max2;
                    }
                }
                else
                {
                    count++;
                    fill = 1;
                }

                if(fill)
                    raster(surface, x1, i, x2, i);

            }
            
            line = line->next;
        }
        
        line = aet;
        while(line)
        {
            if(line->p1_x != line->p2_x)
            {  
                if(line->numerator < 0 || line->denomenator < 0)
                {
                    if(line->numerator < 0)
                        line->increment = line->increment + (0 - line->numerator);
                    else
                        line->increment += line->numerator;
                    
                    if(line->denomenator < 0)
                        sign = 0 - line->denomenator;
                    else
                        sign = line->denomenator;

                    if(line->numerator < 0 && line->denomenator < 0)
                        while(line->increment >= sign)
                        {
                            line->x_min++;
                            line->increment -= sign;
                        }
                    else
                        while(line->increment >= sign)
                        {
                            line->x_min--;
                            line->increment -= sign;
                        }
                }
                else
                {
                    line->increment += line->numerator;
                    while(line->increment >= line->denomenator)
                    {
                        line->x_min++;
                        line->increment -= line->denomenator;
                    }
                }
            }
            line = line->next;
        }
        sort(&aet);
    }
    SDL_UnlockSurface(surface);
}

ET * createEdgeTable()
{
    ET * et = (ET*)malloc(sizeof(ET));
    et->table = (struct bucket**)malloc(sizeof(struct bucket*) * SCREEN_HEIGHT);
    et->max_y = 0;
    et->min_y = 4294967295;
    for(int i = 0; i < SCREEN_HEIGHT; i++)
        et->table[i] = NULL;
    return et;
}

void insertSortedET(Point * p, Point * p_head, ET * edge_table)
{
    if(p == NULL) return;

    struct bucket * curr, * prev, * aux;
    Point * p_end = p->next == NULL ? p_head : p->next;
    int y_max, x_min, pos;

    if(p->y == p_end->y) return;

    if(p->y > p_end->y)
    {
        y_max = p->y;
        x_min = p_end->x;
        if(p_end->y < edge_table->min_y)
            edge_table->min_y = p_end->y;
        if(p->y > edge_table->max_y)
            edge_table->max_y = p->y;
        pos = HASH(p_end->y);
    }
    else
    {
        y_max = p_end->y;
        x_min = p->x;
        if(p->y < edge_table->min_y)
            edge_table->min_y = p->y;
        if(p_end->y > edge_table->max_y)
            edge_table->max_y = p_end->y;
        pos = HASH(p->y);
    }

    if(edge_table->table[pos] == NULL)
    {
        edge_table->table[pos] = (struct bucket*)malloc(sizeof(struct bucket));
        edge_table->table[pos]->x_min = x_min;
        edge_table->table[pos]->y_max = y_max;
        edge_table->table[pos]->p1_x = p->x;
        edge_table->table[pos]->p1_y = p->y;
        edge_table->table[pos]->p2_x = p_end->x;
        edge_table->table[pos]->p2_y = p_end->y;
        edge_table->table[pos]->numerator = (p_end->x - p->x);
        edge_table->table[pos]->denomenator = (p_end->y - p->y);
        edge_table->table[pos]->increment = 0;
        edge_table->table[pos]->next = NULL;
        return;
    }

    curr = prev = edge_table->table[pos];
    while(curr != NULL)
    {
        if(x_min < curr->x_min)
        {
            if(prev == curr)
            {
                aux = (struct bucket*)malloc(sizeof(struct bucket));
                aux->next = curr;
                aux->x_min = x_min;
                aux->y_max = y_max;
                aux->p1_x = p->x;
                aux->p1_y = p->y;
                aux->p2_x = p_end->x;
                aux->p2_y = p_end->y;
                aux->numerator = p_end->x - p->x;
                aux->denomenator = p_end->y - p->y;
                aux->increment = 0;
                edge_table->table[pos] = aux;
                return;
            }
            else
            {
                prev->next = (struct bucket*)malloc(sizeof(struct bucket));
                prev->next->x_min = x_min;
                prev->next->y_max = y_max;
                prev->next->p1_x = p->x;
                prev->next->p1_y = p->y;
                prev->next->p2_x = p_end->x;
                prev->next->p2_y = p_end->y;
                prev->next->numerator = p_end->x - p->x;
                prev->next->denomenator = p_end->y - p->y;
                prev->next->increment = 0;
                prev->next->next = curr;
            }
            return;
        }
        prev = curr;
        curr = curr->next;
    }
    prev->next = (struct bucket*)malloc(sizeof(struct bucket));
    prev->next->x_min = x_min;
    prev->next->y_max = y_max;
    prev->next->p1_x = p->x;
    prev->next->p1_y = p->y;
    prev->next->p2_x = p_end->x;
    prev->next->p2_y = p_end->y;
    prev->next->numerator = p_end->x - p->x;
    prev->next->denomenator = p_end->y - p->y;
    prev->next->increment = 0;
    prev->next->next = NULL;
}

void buildEdgeTable(Polygon * pol, ET * edge_table)
{
    Point * p = pol->head;
    while(p != NULL)
    {
        insertSortedET(p, pol->head, edge_table);
        p = p->next;
    }    
}

void printET(ET * edge_table)
{
    struct bucket * b;
    for(int i = 0; i < SCREEN_HEIGHT; i++)
        if(edge_table->table[i] != NULL)
        {
            printf("{%d}: ", i);
            b = edge_table->table[i];
            while(b != NULL)
            {
                printf("[%d, %d]", b->y_max, b->x_min);
                b = b->next;
            }
            printf("\n");
        } 
}

void ET_test()
{
    ET * edge_table = createEdgeTable();
    Polygon * p = createPoly();
    insertPoint(p, 20, 30);
    insertPoint(p, 70, 10);
    insertPoint(p, 130, 50);
    insertPoint(p, 130, 110);
    insertPoint(p, 70, 70);
    insertPoint(p, 20, 90);
    buildEdgeTable(p, edge_table);
    printET(edge_table);
}

void sort(struct bucket ** aet)
{
    struct bucket * head = *aet, * a, * b, * half, * end;

    if(head == NULL || head->next == NULL)
        return;

    half = head;
    end = head->next;
    while(end != NULL)
    {
        end = end->next;
        if(end != NULL)
        {
            half = half->next;
            end = end->next;
        }
    }
    a = head;
    b = half->next;
    half->next = NULL;
    
    sort(&a);
    sort(&b);

    head = merge(a, b);

    *aet = head;
}

struct bucket * merge(struct bucket * a, struct bucket * b)
{
    struct bucket * merged = NULL;
 
    if (a == NULL)
        return (b);
    else if (b == NULL)
        return (a);

    if (a->x_min <= b->x_min)
    {
        merged = a;
        merged->next = merge(a->next, b);
    }
    else
    {
        merged = b;
        merged->next = merge(a, b->next);
    }
    return (merged);
}