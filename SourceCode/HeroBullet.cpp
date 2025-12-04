#include "HeroBullet.h"
#include "Utils.h"
#include "data/DataCenter.h"
#include "monsters/Monster.h"
#include "shapes/Rectangle.h"

HeroBullet::HeroBullet(const Point &p, const Point &target,
                       const std::string &path, double v, int dmg,
                       double fly_dist)
    : Bullet(p, target, path, v, dmg, fly_dist) {}

void HeroBullet::update() {
  Bullet::update(); // Move the bullet
}
