#ifndef MONSTERSUICIDE_H_INCLUDED
#define MONSTERSUICIDE_H_INCLUDED

#include "Monster.h"
class DateCenter;
// fixed settings: MonsterSuicide attributes
class MonsterSuicide : public Monster {
public:
  MonsterSuicide(const Point &p) : Monster{MonsterType::SUICIDE, p} {
    HP = 10;
    v = 100;
    money = 20;
    bitmap_img_ids[(int)Dir::DOWN] = {0, 1, 2, 3, 4, 5, 6}; // DOWN
  
    bitmap_switch_freq = 5;
  }
  void special_ability(DataCenter* DC) override;
};

#endif
