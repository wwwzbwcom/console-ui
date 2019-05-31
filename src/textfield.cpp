#include "consui.h"
#include <string>

using namespace std;

namespace ConsUI
{
TextFieldAction::TextFieldAction(TextField *tf)
{
    this->tf = tf;
}

void TextFieldAction::action()
{
    int key;

    tf->dirty = true;
    tf->text = "";

    tf->refresh();


    curs_set(2);

    for (;;)
    {
        key = getch();
        bool end = false;
        switch (key)
        {
        case KEY_RESIZE:
        case ERR:
            break;
        case 27:
            Screen::exit();
            break;
        case 9:
        case '\n':
            end = true;
            break;
        case 8:
        case 127:
            if (tf->text.size() > 0)
            {
                tf->text.pop_back();
                tf->refresh();
            }
            break;
        default:
            tf->text += key;
            tf->refresh();
            break;
        }

        if(Screen::getExit())   break;

        if (end)
        {

            if (tf->finish != nullptr)
            {
                tf->finish->action();
            }
            if(Screen::getExit())   break;

            break;
        }
        

        Screen::update();
        
        if(Screen::getExit())   break;

        if (tf->entering != nullptr)
        {
            tf->entering->action();
        }

        if(Screen::getExit())   break;

        //napms(10);
    }

    curs_set(0);

    
}

TextField::~TextField()
{
    if(entering != nullptr)
    {
        delete entering;
        entering = nullptr;
    }

    if(finish != nullptr)
    {
        delete finish;
        finish = nullptr;
    }
}

TextField::TextField(int width, int height, string placeholder, char bindKey, bool secret, Color background, Color foreground)
    : Label(width, height, placeholder, background, foreground)
{
    this->entering = nullptr;
    this->finish = nullptr;

    this->dirty = false;
    this->bindKey = bindKey;
    this->secret = secret;
    action = new TextFieldAction(this);
}

void TextField::onEntering(Action *entering)
{
    this->entering = entering;
}

void TextField::onFinish(Action *finish)
{
    this->finish = finish;
}

string TextField::getText()
{
    if (dirty)
    {
        return text;
    }
    else
    {
        return "";
    }
}

void TextField::draw()
{
    if (focus)
    {
        swap(background, foreground);
    }

    updateSize();
    Rect::draw();

    short pair = get_color_pair(background, foreground);

    int x = this->x; // + (actualWidth - this->text.size()) / 2;
    int y = this->y + (actualHeight / 2);

    x = this->x + 1;
    attron(COLOR_PAIR(pair));

    if(bindKey != 0)
    {
        mvprintw(y, x, "[%c] ", bindKey);
    }

    attron(COLOR_PAIR(pair) | A_UNDERLINE);

    if (bindKey == 0)
    {
        for (x = this->x + 3; x < this->x + this->actualWidth - 3; x++)
        {
            mvprintw(y, x, " ");
        }
    }
    else
    {
        for (x = this->x + 5; x < this->x + this->actualWidth - 3; x++)
        {
            mvprintw(y, x, " ");
        }
    }
    

    if (dirty)
    {
        attron(A_DIM);
    }
    else
    {
        attron(A_BOLD);
    }

    

    if (dirty)
    {
        attroff(A_DIM);
    }
    else
    {
        attroff(A_BOLD);
    }

    x = this->x + (actualWidth - this->text.size()) / 2;

    string show;
    if (secret && dirty)
    {
        show = string(text.length(), '*');
    }
    else
    {
        show = text;
    }

    if (text.size() > this->actualWidth - 6)
    {
        show = show.substr(0, this->actualWidth - 6);
    }
    x = this->x + (actualWidth - show.size()) / 2;
    mvprintw(y, x, show.data());

    attroff(COLOR_PAIR(pair) | A_UNDERLINE);

    if (focus)
    {
        swap(background, foreground);
    }
}

} // namespace ConsUI