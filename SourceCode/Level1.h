#ifndef LEVEL1_H_INCLUDED
#define LEVEL1_H_INCLUDED

#include "shapes/Rectangle.h"
#include <vector>

#include <allegro5/allegro.h>

constexpr char level1_image_path[] = "./assets/image/scene/Level1.jpg";

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
