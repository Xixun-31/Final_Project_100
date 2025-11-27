#ifndef MONSTERELITE_H_INCLUDED
#define MONSTERELITE_H_INCLUDED

#include "Monster.h"

// fixed settings: MonsterElite attributes
class MonsterElite : public Monster {
public:
  MonsterElite(const Point &p) : Monster{MonsterType::ELITE, p} {
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
