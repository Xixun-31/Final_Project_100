#ifndef WIN_H_INCLUDED
#define WIN_H_INCLUDED

constexpr char win_image_path[] = "./assets/image/scene/WIN.jpg";

#include <allegro5/allegro.h>

class Win {
public:
  Win();
  void init();
  void update();
  void draw();

private:
  ALLEGRO_BITMAP *background = nullptr;
};

#endif
