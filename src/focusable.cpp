

#include "consui.h"
#include <string>

using namespace std;

namespace ConsUI
{

Focusable::~Focusable()
{
    if(action != nullptr)
    {
        delete action;
        action = nullptr;
    }
}

Focusable::Focusable()
{
    focus = false;
    bindKey = 0;
    action = nullptr;
}

void Focusable::startFocus()
{
    focus = true;
}

void Focusable::stopFocus()
{
    focus = false;
}


};