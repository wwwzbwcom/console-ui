#include "consui.h"
#include <string>

using namespace std;

namespace ConsUI
{

Button::Button(int width, int height, string text, Action *action, char bindKey, Color background, Color foreground) 
: Label(width, height, text, background, foreground)
{
    this->focus = false;
    this->bindKey = bindKey;
    this->action = action;
}

void Button::draw()
{
    if(focus)
    {
        swap(background, foreground);
    }

    updateSize();
    Rect::draw();

    short pair = get_color_pair(background, foreground);

    int x = this->x + 2;// + (actualWidth - this->text.size()) / 2;
    int y = this->y + (actualHeight / 2);

    attron(COLOR_PAIR(pair));
    mvprintw(y, x, "[%c] ", bindKey);
    
    x = this->x + (actualWidth - this->text.size()) / 2;
    mvprintw(y, x, text.data());
    attroff(COLOR_PAIR(pair));

    if(focus)
    {
        swap(background, foreground);
    }
}


};