#ifndef LEVEL3_H_INCLUDED
#define LEVEL3_H_INCLUDED

#include "shapes/Rectangle.h"
#include <allegro5/allegro.h>
#include <vector>

class Level3 {
public:
  Level3();
  void init();
  void update();
  void draw();

private:
  ALLEGRO_BITMAP *background = nullptr;
};

#endif
