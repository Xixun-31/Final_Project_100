#ifndef MONSTERWOLFKNIGHT_H_INCLUDED
#define MONSTERWOLFKNIGHT_H_INCLUDED

#include "Monster.h"

// fixed settings: MonsterWolfKnight attributes
class MonsterWolfKnight : public Monster {
public:
  MonsterWolfKnight(const Point &p) : Monster{MonsterType::WOLFKNIGHT, p} {
    HP = 15;
    v = 80;
    money = 30;
    bitmap_img_ids[0] = {0, 1, 2, 3}; // UP
    bitmap_img_ids[1] = {0, 1, 2, 3}; // DOWN
    bitmap_img_ids[2] = {0, 1, 2, 3}; // LEFT
    bitmap_img_ids[3] = {0, 1, 2, 3}; // RIGHT
    bitmap_switch_freq = 20;
  }
};

#endif
