#ifndef LEVEL1_H_INCLUDED
#define LEVEL1_H_INCLUDED

#include "shapes/Rectangle.h"
#include <vector>

#include <allegro5/allegro.h>



class Level1 {
public:
  Level1();
  void init();
  void update();
  void draw();

private:
  ALLEGRO_BITMAP *background = nullptr;
};

#endif
