#ifndef MONSTERWOLF_H_INCLUDED
#define MONSTERWOLF_H_INCLUDED

#include "Monster.h"

// fixed settings: MonsterWolf attributes
class MonsterWolf : public Monster {
public:
  MonsterWolf(const Point &p) : Monster{MonsterType::WOLF, p} {
    HP = 10;
    v = 60;
    money = 10;
    bitmap_img_ids[0] = {0, 1, 2, 3};    // UP
    bitmap_img_ids[1] = {0, 1, 2, 3};    // DOWN
    bitmap_img_ids[2] = {0, 1, 2, 3, 4}; // LEFT
    bitmap_img_ids[3] = {0, 1, 2, 3, 4}; // RIGHT
    bitmap_switch_freq = 20;
  }
};

#endif
