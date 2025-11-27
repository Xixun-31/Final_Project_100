#ifndef LOSE_H_INCLUDED
#define LOSE_H_INCLUDED

#include <allegro5/allegro.h>

class Lose {
public:
  Lose();
  void init();
  void update();
  void draw();

private:
  ALLEGRO_BITMAP *background = nullptr;
};

#endif
