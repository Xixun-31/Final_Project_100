#ifndef MONSTERDEMONNINJA_H_INCLUDED
#define MONSTERDEMONNINJA_H_INCLUDED

#include "Monster.h"

// fixed settings: MonsterDemonNinja attributes
class DataCenter;
class MonsterDemonNinja : public Monster {
public:
  MonsterDemonNinja(const Point &p) : Monster{MonsterType::DEMONNINJA, p} {
    HP = 5;
    v = 60;
    money = 40;
    bitmap_img_ids[(int)Dir::LEFT] = {0, 1, 2, 3}; // DOWN
    bitmap_img_ids[(int)Dir::RIGHT] = {0, 1, 2, 3}; // DOWN
    bitmap_switch_freq = 20;
  }
  void special_ability(DataCenter* DC) override;
  void update() override;
  void draw() override;
  double slap_cd = 0.0; // 秒
  bool was_overlapping = false;
  bool is_attacking = false;
  double attack_timer = 0.0;     // 還要攻擊多久
  double attack_cd = 0.0;        // 冷卻
  int attack_frame = 0;
  double attack_anim_timer = 0.0;


};

#endif
