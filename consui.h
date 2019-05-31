#include <ncurses.h>
#include <string>
#include <list>

using namespace std;

#ifndef CONSUI
#define CONSUI

namespace ConsUI
{

class Canvas;
class Rect;
class Screen;

class Action;
class Focusable;
class TextFieldAction;
class Button;
class TextField;

enum Color
{
  Alpha = -1,
  Black = 0,
  Red = 1,
  Green = 2,
  Yellow = 3,
  Blue = 4,
  Magenta = 5,
  Cyan = 6,
  White = 7
};

void init_color_pairs();
int get_color_pair(Color background = Black, Color foreground = White);

enum Alignment
{
  Left = 0,
  Center = 1,
  Right = 2,
  Distributed = 3
};

class Rect
{
  friend class Screen;
  friend class Canvas;

protected:
  Canvas *parent;

  int x, y;
  int width, height;

  int actualWidth, actualHeight;

  Color background, foreground;

  virtual void updateSize();
  virtual void draw();

  void setPos(int x, int y);
  void setSize(int width, int height);

public:
  void refresh();
  Rect();
  Rect(int width, int height, Color background = Black);
};

class Canvas : public Rect
{
  friend class Screen;

private:
  Rect *map[128][128];
  Alignment rowAlignments[128];
  int rowLength[128];

  void (*d)(Canvas *canvas);
  int headerEnd = 0, tailerBegin = -1;

  int rows, cols;
  int currentMode;
  int calcHeight(int start, int end);
  int calcWidth(int row);
  void drawRange(int start, int end, int height);
  void drawBackground();

  void updateSize() override;
  void draw() override;
  void init();
  void destruct();

  Action *start;
  Action *update;

protected:
  bool hasDescribe;
  void parseDescription();
  
  virtual void description(Canvas *canvas);

public:
  ~Canvas();
  Canvas();
  Canvas(void (*description)(Canvas *canvas), Color background = Black, int width = -100, int height = -100);

  void setBackground(Color background);
  void add(Rect *rect, Alignment alignment = Center);

  void endHeader();
  void beginTailer();

  void beginHorizontal(Alignment alignment = Center);
  void endHorizontal();

  void onStart(Action *action);
  void onUpdate(Action *action);
};

class Screen
{

private:
  static Focusable *keyBinddings[256];
  static Canvas *root;
  static bool hasExit;

  static void clearKeyBindings();
  static void addKeyBindings(Canvas *canvas);
  static void processKey(char key);

public:
  static void focusOn(Focusable *f);
  static void initRoot(Canvas *canvas);
  static void setRoot(Canvas *canvas);
  static bool checkRoot(Canvas *canvas);
  static void refresh();
  static void update();

  static void exit();
  static bool getExit();
};

class Padding : public Rect
{
public:
  Padding(int width, int height);
};

class Label : public Rect
{
protected:
  string text;
  void draw() override;

public:
  Label(int width, int height, string text, Color background = Black, Color foreground = White);
  void setText(string text);
};

class Action
{

private:
  void (*act)();

public:
  virtual void action();
  Action();
  Action(void (*action)());
};

class Actions : public Action
{
  private:
    Action *a, *b;
  public:
    void action() override;
    Actions(Action *a, Action *b);
};

template<class T>
class SwitchCanvasAction : public Action
{
  public:
    void action() override;
};

template <class T>
void SwitchCanvasAction<T>::action()
{
    Screen::setRoot(new T());
}

class Focusable
{
  friend class Screen;

protected:
  bool focus;
  char bindKey;
  Action *action;
  virtual void startFocus();
  virtual void stopFocus();
  ~Focusable();
  Focusable();
};

class Button : public Label, public Focusable
{
protected:
  void draw() override;

public:
  Button(int width, int height, string text, Action *action = nullptr, char bindKey = 0, Color background = Black, Color foreground = White);
};

class TextFieldAction : public Action
{
private:
  TextField *tf;

public:
  void action() override;
  TextFieldAction(TextField *tf);
};

class TextField : public Label, public Focusable
{
  friend class TextFieldAction;

protected:
  void draw() override;
  string placeholder;
  bool dirty;
  bool secret;
  Action *finish, *entering;

public:
  ~TextField();
  TextField(int width, int height, string placeholder, char bindKey = 0, bool secret = false, Color background = Black, Color foreground = White);
  string getText();
  void onEntering(Action *entering);
  void onFinish(Action *finish);
};

} // namespace ConsUI

#endif