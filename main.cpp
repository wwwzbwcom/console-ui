#include "consui.h"

using namespace ConsUI;

void description(Canvas *cv)
{
    cv->setBackground(Cyan);

    cv->add(new Label(-100, 5, "TITLE", Magenta, White));
    cv->endHeader();

    cv->add(new TextField(-40, 3, "username", 'u', false, White, Blue));
    cv->add(new TextField(-40, 3, "password", 'p', true, White, Blue));
    
    cv->add(new Padding(-40, 3));

    cv->beginHorizontal();
    cv->add(new Button(-40, 3, "login", nullptr, 'l', Blue, White));
    cv->add(new Button(-40, 3, "regist", nullptr, 'r', Green, White));
    cv->endHorizontal();

    cv->beginTailer();
    cv->add(new Label(-100, 5, "TAILER", Red, White));
}

int main()
{
    Screen::initRoot(new Canvas(description));
}