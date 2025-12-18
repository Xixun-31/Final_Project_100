#ifndef LOSE_H_INCLUDED
#define LOSE_H_INCLUDED

#include <allegro5/allegro.h>
#include "shapes/Rectangle.h"

class Lose {
public:
  Lose();
  void init();
  void update();
  void draw();

  int hover_btn; // 0: None, 1: Menu, 2: Exit
private:
  ALLEGRO_BITMAP *background = nullptr;
  Rectangle menu_btn;
  Rectangle exit_btn;
};

#endif
