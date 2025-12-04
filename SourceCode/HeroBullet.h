#ifndef HEROBULLET_H_INCLUDED
#define HEROBULLET_H_INCLUDED

#include "towers/Bullet.h"

class HeroBullet : public Bullet {
public:
  HeroBullet(const Point &p, const Point &target, const std::string &path,
             double v, int dmg, double fly_dist);
  void update();
  // draw is inherited from Bullet
};

#endif
