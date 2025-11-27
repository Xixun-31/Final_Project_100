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

private:
  ALLEGRO_BITMAP *menu_img;
};

#endif
