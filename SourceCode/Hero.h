#ifndef HERO_H_INCLUDED
#define HERO_H_INCLUDED

#include "Object.h"
#include <map>
#include <string>

enum class HeroState { LEFT, RIGHT, UP, DOWN, HeroState_MAX };

class Hero : public Object {
public:
  void init();
  void update();
  void draw() override;
  void attack();
  void hit();
  bool is_invincible();

private:
  HeroState state = HeroState::UP;
  std::map<HeroState, std::string> gif;
  double speed = 3;
  int counter = 0;
  int attack_freq = 20; // Cooldown frames
  int invincible_timer = 0;
  int roll_timer = 0;
  int roll_duration = 15;
  int roll_cd = 0;
  int roll_cd_duration = 60;
  int roll_delay_timer = 0;
  int roll_delay_duration = 20; // Stiffness after roll
  double roll_speed = 8.0;
  HeroState roll_direction = HeroState::UP;
};

#endif
