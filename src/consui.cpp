#include "consui.h"

namespace ConsUI
{

void init_color_pairs()
{
    for (short f = 0; f <= 7; f++)
    {
        for (short b = 0; b <= 7; b++)
        {
            int p = get_color_pair((Color)b, (Color)f);
            init_pair(p, f, b);
        }
    }
}

int get_color_pair(Color background, Color foreground)
{
    return background * 8 + foreground + 1;
}

}