#ifndef MONSTERSLIME_H_INCLUDED
#define MONSTERSLIME_H_INCLUDED

#include "Monster.h"
class DataCenter;
// fixed settings: MonsterSlime attributes
class MonsterSlime : public Monster {
public:
  MonsterSlime(const Point &p) : Monster{MonsterType::SLIME, p} {
    HP = 5;
    v = 40;
    money = 20;
   
    bitmap_img_ids[(int)Dir::LEFT] = {0, 1, 2, 3, 4, 5, 6, 7}; // LEFT
    bitmap_img_ids[(int)Dir::RIGHT] = {0, 1, 2, 3, 4, 5, 6, 7}; // RIGHT
    bitmap_switch_freq = 20;
  }
  
  void special_ability(DataCenter* DC) override;
  bool splited = false;
};

#endif
