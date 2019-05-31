#include "consui.h"

namespace ConsUI
{

Action::Action()
{
    act = nullptr;
}

Action::Action(void (*action)())
{
    act = action;
}

void Action::action()
{
    if(act != nullptr)
    {
        act();
    }
}

Actions::Actions(Action *a, Action *b)
{
    this->a = a;
    this->b = b;
}

void Actions::action()
{
    a->action();

    if(Screen::getExit())   return;

    b->action();
}







}; // namespace ConsUI