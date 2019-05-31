#include "consui.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <cmath>
#include <cstring>

using namespace std;

namespace ConsUI
{

void Canvas::init()
{
    for (int i = 0; i < 128; i++)
        for (int j = 0; j < 128; j++)
        {
            map[i][j] = nullptr;
        }

    this->start = nullptr;
    this->update = nullptr;
    
    hasDescribe = false;
}

void Canvas::destruct()
{
    for (int i = 0; i < 128; i++)
        for (int j = 0; j < 128; j++)
        {
            if (map[i][j] != nullptr)
            {
                delete map[i][j];
                map[i][j] = nullptr;
            }
        }

    if(start != nullptr)
    {
        delete start;
        start = nullptr;
    }

    if(update != nullptr)
    {
        delete update;
        update = nullptr;
    }
}
Canvas::Canvas()
{
    init();

    this->width = -100;
    this->height = -100;
    this->d = nullptr;
    this->background = Black;
}

Canvas::Canvas(void (*description)(Canvas *canvas), Color background, int width, int height) : Canvas()
{
    init();

    this->width = width;
    this->height = height;
    this->d = description;
    this->background = background;
}

Canvas::~Canvas()
{
    destruct();    
}



void Canvas::parseDescription()
{
    if(hasDescribe == true)
    {
        destruct();
    }
    hasDescribe = true;
    memset(rowLength, 0, sizeof(rowLength));
    rows = 0;
    cols = 0;
    currentMode = 0;
    description(this);
    if (tailerBegin == -1)
    {
        tailerBegin = rows;
    }
}

void Canvas::description(Canvas *canvas)
{
    if (d != nullptr)
    {
        d(canvas);
    }
}

void Canvas::drawBackground()
{
    if (background != Alpha)
    {
        short pair = get_color_pair(background);
        attron(COLOR_PAIR(pair));
        for (int y = this->y; y < this->y + actualHeight; y++)
        {
            for (int x = this->x; x < this->x + actualWidth; x++)
            {
                mvprintw(y, x, " ");
            }
        }
        attroff(COLOR_PAIR(pair));
    }
}

int Canvas::calcHeight(int start, int end)
{
    int h = 0;

    for (int r = start; r < end; r++)
    {
        int my = 0;
        for (int c = 0; c < rowLength[r]; c++)
        {
            my = max(my, map[r][c]->actualHeight);
        }
        h += my;
    }

    return h;
}

int Canvas::calcWidth(int row)
{
    int w = 0;
    for (int c = 0; c < rowLength[row]; c++)
    {
        w += map[row][c]->actualWidth;
    }

    return w;
}

void Canvas::drawRange(int start, int end, int actualHeight)
{
    int y;
    int x = 0;
    y = actualHeight;
    for (int r = start; r < end; r++)
    {
        int my = 0;
        int w = calcWidth(r);
        switch (rowAlignments[r])
        {
        case Left:
            x = 0; //this->x;
            break;
        case Center:
            x = floor((actualWidth - w) / 2.0f);
            break;
        case Right:
            x = (actualWidth - w);
            break;
        default:
            break;
        }

        x += this->x;

        for (int c = 0; c < rowLength[r]; c++)
        {
            map[r][c]->setPos(x, y);
            map[r][c]->draw();
            x += map[r][c]->actualWidth;
            my = max(my, map[r][c]->actualHeight);
        }
        y += my;
    }
}

void Canvas::updateSize()
{
    Rect::updateSize();
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < rowLength[r]; c++)
        {
            map[r][c]->updateSize();
        }
    }
}

void Canvas::draw()
{
    if (hasDescribe == false)
    {
        parseDescription();
    }
    updateSize();
    drawBackground();

    drawRange(0, headerEnd, y);

    int h;
    h = calcHeight(headerEnd, tailerBegin);
    drawRange(headerEnd, tailerBegin, y + floor((actualHeight - h) / 2.0f));

    h = calcHeight(tailerBegin, rows);
    drawRange(tailerBegin, rows, y + floor(actualHeight - h));
}

void Canvas::setBackground(Color background)
{
    this->background = background;
}

void Canvas::add(Rect *rect, Alignment alignment)
{
    map[rows][cols] = rect;
    rect->parent = this;

    if (currentMode == 0)
    {
        rowAlignments[rows] = alignment;
        rowLength[rows] = 1;
        cols = 0;
        rows++;
    }
    else
    {
        cols++;
    }
}

void Canvas::endHeader()
{
    headerEnd = rows;
}

void Canvas::beginTailer()
{
    tailerBegin = rows;
}

void Canvas::beginHorizontal(Alignment alignment)
{
    rowAlignments[rows] = alignment;
    cols = 0;
    currentMode = 1;
}

void Canvas::endHorizontal()
{
    currentMode = 0;
    rowLength[rows] = cols;

    rows++;
    cols = 0;
}

void Canvas::onStart(Action *action)
{
    start = action;
}

void Canvas::onUpdate(Action *action)
{
    update = action;
}

} // namespace ConsUI