#ifndef LEVEL0_H_INCLUDED
#define LEVEL0_H_INCLUDED

#include <allegro5/allegro.h>

class Level0 {
public:
  Level0();
  void init();
  void update();
  void draw();

private:
  ALLEGRO_BITMAP *background = nullptr;
};

#endif
