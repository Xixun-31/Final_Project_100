#ifndef MONSTERSMALLWOLF_H_INCLUDED
#define MONSTERSMALLWOLF_H_INCLUDED

#include "Monster.h"

// fixed settwwwwwwwwwwwsadings: MonsterSmallWolf attributes
class MonsterSmallWolf : public Monster {
public:
  MonsterSmallWolf(const Point &p) : Monster{MonsterType::SMALLWOLF, p} {
    HP = 5;
    v = 40;
    money = 20;
   
    bitmap_img_ids[(int)Dir::LEFT] = {0, 1, 2, 3, 4, 5, 6, 7}; // LEFT
    bitmap_img_ids[(int)Dir::RIGHT] = {0, 1, 2, 3, 4, 5, 6, 7}; // RIGHT
    bitmap_switch_freq = 20;
  }
};

#endif
