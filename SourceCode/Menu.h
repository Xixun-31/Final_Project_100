#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include "shapes/Rectangle.h"
#include <allegro5/allegro.h>
#include <vector>

class Menu {
public:
  Menu();
  void init();
  void update();
  void draw();

  int hover_btn; // 0: None, 1: Start, 2: About, 3: Exit
private:
  ALLEGRO_BITMAP *menu_img;
  Rectangle start_btn;
  Rectangle about_btn;
  Rectangle exit_btn;
};

#endif
