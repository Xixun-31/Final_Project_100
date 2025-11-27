#ifndef LEVEL2_H_INCLUDED
#define LEVEL2_H_INCLUDED

#include "shapes/Rectangle.h"
#include <vector>

#include <allegro5/allegro.h>

constexpr char level2_image_path[] = "./assets/image/scene/Level2.jpg";

class Level2 {
public:
  Level2();
  void init();
  void update();
  void draw();

private:
  ALLEGRO_BITMAP *background = nullptr;
};

#endif
