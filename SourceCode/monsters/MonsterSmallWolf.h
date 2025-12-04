#ifndef MONSTERSMALLWOLF_H_INCLUDED
#define MONSTERSMALLWOLF_H_INCLUDED

#include "Monster.h"

// fixed settings: MonsterSmallWolf attributes
class MonsterSmallWolf : public Monster {
public:
  MonsterSmallWolf(const Point &p) : Monster{MonsterType::SMALLWOLF, p} {
    HP = 25;
    v = 25;
    money = 5;
    bitmap_img_ids[0] = {0, 1, 2, 3}; // UP
    bitmap_img_ids[1] = {0, 1, 2, 3}; // DOWN
    bitmap_img_ids[2] = {0, 1, 2, 3}; // LEFT
    bitmap_img_ids[3] = {0, 1, 2, 3}; // RIGHT
    bitmap_switch_freq = 20;
  }
};

#endif
