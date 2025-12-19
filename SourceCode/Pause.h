#ifndef PAUSE_H_INCLUDED
#define PAUSE_H_INCLUDED

#include "shapes/Rectangle.h"
#include <allegro5/allegro.h>

class Pause {
public:
    Pause();
    void init();
    void update();
    void draw();

    int hover_btn; // 0: None, 1: Resume, 2: Menu, 3: Exit

private:
    Rectangle resume_btn;
    Rectangle menu_btn;
    Rectangle exit_btn;
};

#endif
