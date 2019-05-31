#include "consui.h"
#include <cmath>
#include <algorithm>

using namespace std;

namespace ConsUI
{

Rect::Rect()
{
    parent = nullptr;

    width = 0;
    height = 0;
    x = 0;
    y = 0;
    background = Black;
    foreground = White;
}

Rect::Rect(int width, int height, Color background)
{
    parent = nullptr;

    this->width = width;
    this->height = height;
    x = 0;
    y = 0;

    this->background = background;
}

void Rect::updateSize()
{
    int pw, ph;
    if(parent != nullptr)
    {
        pw = parent->actualWidth;
        ph = parent->actualHeight;
    }
    else
    {
        pw = COLS;
        ph = LINES;
    }
    if(width < 0)
    {
        actualWidth = ceil(pw * (-width) / 100.0);
    }
    else
    {
        actualWidth = width;
    }

    if(height < 0)
    {
        actualHeight = ceil(ph * (-height) / 100.0);
    }
    else
    {
        actualHeight = height;
    }
}

void Rect::draw()
{
    updateSize();

    if(background != Alpha)
    {

        short pair = get_color_pair(background);
        attron(COLOR_PAIR(pair));
        for (int yt = y; yt < y + actualHeight; yt++)
        {
            for (int xt = x; xt < x + actualWidth; xt++)
            {
                mvprintw(yt, xt, " ");
            }
        }
        attroff(COLOR_PAIR(pair));
    }

    
}

void Rect::refresh()
{
    /*
    if(parent != nullptr)
    {
        parent->refresh();
    }
    */
    this->draw();
}

void Rect::setPos(int x, int y)
{
    this->x = x;
    this->y = y;
}

void Rect::setSize(int width, int height)
{
    this->width = width;
    this->height = height;
}

} // namespace ConsUI