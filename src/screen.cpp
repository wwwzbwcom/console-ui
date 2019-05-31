#include "consui.h"
#include <list>
#include <cmath>
#include <algorithm>
#include <unistd.h>

using namespace std;

namespace ConsUI
{

Focusable *Screen::keyBinddings[256];
Canvas *Screen::root = nullptr;
bool Screen::hasExit = false;

void Screen::initRoot(Canvas *canvas)
{
    initscr();
    raw();
    noecho();
    nodelay(stdscr, TRUE);
    start_color();
    init_color_pairs();
    curs_set(0);

    Screen::setRoot(canvas);
    delete root;

    flushinp();
    echo();
    endwin();
}

bool Screen::checkRoot(Canvas *canvas)
{
    return (canvas == root);
}

void Screen::setRoot(Canvas *canvas)
{
    if (root != nullptr)
    {
        delete root;
        root = nullptr;
    }
    root = canvas;

    root->parseDescription();
    root->refresh();

    if (root->start != nullptr)
    {
        root->start->action();
    }

    if (hasExit)    return;

    clearKeyBindings();
    addKeyBindings(root);

    int key;
    for (;;)
    {
        key = getch();
            

        switch (key)
        {
        case KEY_RESIZE:
        case ERR:
            break;
        case 27:
            exit();
            break;
        default:
            processKey(key);
            break;
        }

        if (hasExit)    return;

        napms(10);
        if (root->update != nullptr)
        {
            root->update->action();
        }

        if (hasExit)    return;

        refresh();
        mvprintw(0, 0, "%d", key);
    }

    return;
}

void Screen::refresh()
{
    if (root != nullptr)
    {
        root->refresh();
    }
}

void Screen::exit()
{
    hasExit = true;
}

bool Screen::getExit()
{
    return hasExit;
}

void Screen::update()
{
    if (root->update != nullptr)
    {
        root->update->action();
    }
}

void Screen::clearKeyBindings()
{
    for (int i = 0; i < 256; i++)
    {
        keyBinddings[i] = nullptr;
    }
}

void Screen::addKeyBindings(Canvas *canvas)
{
    for (int r = 0; r < canvas->rows; r++)
    {
        for (int c = 0; c < canvas->rowLength[r]; c++)
        {
            Focusable *f = dynamic_cast<Focusable *>(canvas->map[r][c]);
            if (f != nullptr)
            {
                keyBinddings[f->bindKey] = f;
            }
        }
    }
}

void Screen::focusOn(Focusable *f)
{
    f->startFocus();
    root->refresh();

    if (f->action != nullptr)
    {
        f->action->action();
    }

    if (hasExit)    return;
    
    f->stopFocus();
    root->refresh();
    
}

void Screen::processKey(char key)
{
    Focusable *f = keyBinddings[key];
    if (f != nullptr)
    {
        focusOn(f);
    }
}

} // namespace ConsUI