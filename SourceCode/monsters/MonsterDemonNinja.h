#ifndef MONSTERDEMONNINJA_H_INCLUDED
#define MONSTERDEMONNINJA_H_INCLUDED

#include "Monster.h"

// fixed settings: MonsterDemonNinja attributes
class DataCenter;
class MonsterDemonNinja : public Monster {
public:
  MonsterDemonNinja(const Point &p) : Monster{MonsterType::DEMONNINJA, p} {
    HP = 50;
    v = 60;
    money = 40;
    bitmap_img_ids[(int)Dir::LEFT] = {0, 1, 2, 3}; // DOWN
    bitmap_img_ids[(int)Dir::RIGHT] = {0, 1, 2, 3}; // DOWN
    bitmap_switch_freq = 20;
  }
  void special_ability(DataCenter* DC) override;


  double next_slap_time = 0.0;
  bool was_overlapping = false;

};

#endif
