#ifndef MONSTERDEMONNINJA_H_INCLUDED
#define MONSTERDEMONNINJA_H_INCLUDED

#include "Monster.h"

// fixed settings: MonsterDemonNinja attributes
class MonsterDemonNinja : public Monster {
public:
  MonsterDemonNinja(const Point &p) : Monster{MonsterType::DEMONNINJA, p} {
    HP = 50;
    v = 60;
    money = 40;
    bitmap_img_ids[0] = {0, 1, 2, 3}; // UP
    bitmap_img_ids[1] = {0, 1, 2, 3}; // DOWN
    bitmap_img_ids[2] = {0, 1, 2, 3}; // LEFT
    bitmap_img_ids[3] = {0, 1, 2, 3}; // RIGHT
    bitmap_switch_freq = 20;
  }
};

#endif
