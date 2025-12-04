#ifndef HERO_H_INCLUDED
#define HERO_H_INCLUDED

#include "Object.h"
#include <map>
#include <string>
#include <vector>

enum class HeroState { LEFT, RIGHT, UP, DOWN, HeroState_MAX };

class Hero : public Object {
public:
  void init();
  void update();
  void draw() override;
  void attack();
  void hit();
  bool is_invincible();

  bool is_death_anim_done() const {
    return is_dying && animation_frame >= 4 && death_anim_timer >= 60;
  }
  bool is_dying = false;

  int get_ammo() const { return ammo; }
  bool get_is_reloading() const { return is_reloading; }
  int get_reload_timer() const { return reload_timer; }
  int get_reload_duration() const { return reload_duration; }
  void reload();

private:
  HeroState state = HeroState::UP;
  std::map<HeroState, std::string> img;
  std::map<HeroState, std::vector<std::string>> run_img;
  std::vector<std::string> shoot_img;
  std::vector<std::string> dead_img;
  std::vector<std::string> prepare_img;
  std::string slide_img;
  std::string pain_img;
  std::string delay_img;

  int animation_tick = 0;
  int animation_frame = 0;
  bool is_moving = false;
  bool is_attacking = false;
  int attack_anim_timer = 0;
  int death_anim_timer = 0;
  bool is_pain = false;

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

  // Ammo system
  int ammo = 10;
  const int max_ammo = 10;
  bool is_reloading = false;
  int reload_timer = 0;
  const int reload_duration = 60; // 1 second
  // Bomb system
  bool is_bombing = false;
  int bomb_timer = 0;
  const int bomb_duration = 10; // Reduced duration
  std::vector<std::string> boom_img;
  int bomb_count = 3;
  const int max_bomb_count = 3;
  void activate_bomb();

public:
  int get_bomb_count() const { return bomb_count; }
};

#endif
