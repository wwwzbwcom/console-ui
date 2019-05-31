#include "consui.h"
#include <string>

using namespace std;
using namespace ConsUI;

Label::Label(int width, int height, string text, Color background, Color foreground)
{
    this->width = width;
    this->height = height;
    this->text = text;
    this->background = background;
    this->foreground = foreground;
}


void Label::draw()
{
    updateSize();
    Rect::draw();

    short pair = get_color_pair(background, foreground);
    attron(COLOR_PAIR(pair));
    int x = this->x + (actualWidth - this->text.size()) / 2;
    int y = this->y + (actualHeight / 2);
    mvprintw(y, x, text.data());
    attroff(COLOR_PAIR(pair));
}

void Label::setText(string text)
{
    this->text = text;
}
