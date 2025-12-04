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

private:
  HeroState state = HeroState::UP;
  std::map<HeroState, std::string> gif;
  double speed = 3;
  int counter = 0;
  int attack_freq = 20; // Cooldown frames
};

#endif
