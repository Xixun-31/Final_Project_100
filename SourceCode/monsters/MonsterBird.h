#ifndef MONSTERBIRD_H_INCLUDED
#define MONSTERBIRD_H_INCLUDED

#include "Monster.h"

// fixed settings: MonsterBird attributes
class MonsterBird : public Monster {
public:
  MonsterBird(const Point &p) : Monster{MonsterType::BIRD, p} {
    HP = 25;
    v = 40;
    money = 20;
    bitmap_img_ids[0] = {0, 1, 2, 3}; // UP
    bitmap_img_ids[1] = {0, 1, 2, 3}; // DOWN
    bitmap_img_ids[2] = {0, 1, 2, 3}; // LEFT
    bitmap_img_ids[3] = {0, 1, 2, 3}; // RIGHT
    bitmap_switch_freq = 20;
  }
};

#endif
