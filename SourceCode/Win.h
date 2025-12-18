#ifndef WIN_H_INCLUDED
#define WIN_H_INCLUDED

constexpr char win_image_path[] = "./assets/image/scene/WIN.jpg";

#include <allegro5/allegro.h>
#include "shapes/Rectangle.h"

class Win {
public:
  Win();
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
