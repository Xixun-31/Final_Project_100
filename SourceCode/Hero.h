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

private:
  HeroState state = HeroState::UP;
  std::map<HeroState, std::string> gif;
  double speed = 5;
};

#endif
