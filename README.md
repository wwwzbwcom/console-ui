# console-ui
Pure C++, Lite weight UI Kit for POSIX terminal.

## Describe UI by Pure C++ language

With simple code like this, you can describe an interface.

```cpp
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
```

- Event Driven
