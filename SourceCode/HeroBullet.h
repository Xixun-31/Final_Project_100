#ifndef HEROBULLET_H_INCLUDED
#define HEROBULLET_H_INCLUDED

#include "towers/Bullet.h"

class HeroBullet : public Bullet {
public:
  HeroBullet(const Point &p, const Point &target, const std::string &path,
             double v, int dmg, double fly_dist, bool animated = false);
  void update();
  void draw(); // Custom draw for scaling
private:
  bool is_animated;
  int anim_timer;
  ALLEGRO_BITMAP *bmp1;
  ALLEGRO_BITMAP *bmp2;
};

#endif
