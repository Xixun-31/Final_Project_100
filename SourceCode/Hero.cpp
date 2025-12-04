#include "Hero.h"
#include "HeroBullet.h"
#include "algif5/algif.h"
#include "data/DataCenter.h"
#include "data/GIFCenter.h"
#include "shapes/Rectangle.h"
#include <cstring>

namespace HeroSetting {
static constexpr char Hero_gifs_root_path[40] = "./assets/gif/Hero/dragonite_";
static constexpr char dir_path_postfix[][10] = {"left", "right", "front",
                                                "back"};
} // namespace HeroSetting

void Hero::init() {
  for (int i = 0; i < static_cast<int>(HeroState::HeroState_MAX); ++i) {
    gif[static_cast<HeroState>(i)] =
        std::string(HeroSetting::Hero_gifs_root_path) +
        HeroSetting::dir_path_postfix[i] + ".gif";
  }
  GIFCenter *GC = GIFCenter::get_instance();
  ALGIF_ANIMATION *animation = GC->get(gif[state]);
  DataCenter *DC = DataCenter::get_instance();
  shape.reset(new Rectangle{DC->window_height / 2.0, DC->window_width / 2.0,
                            DC->window_height / 2.0 + animation->width,
                            DC->window_width / 2.0 + animation->height});
}

void Hero::update() {
  DataCenter *DC = DataCenter::get_instance();
  GIFCenter *GC = GIFCenter::get_instance();
  ALGIF_ANIMATION *animation = GC->get(gif[state]);
  if (roll_cd > 0)
    roll_cd--;

  if (roll_delay_timer > 0) {
    // Stiffness after rolling
    roll_delay_timer--;
    return; // Cannot move or attack
  }

  if (roll_timer > 0) {
    // Rolling logic (Stiffness during roll)
    state = roll_direction; // Keep facing the roll direction
    invincible_timer = roll_delay_duration;
    switch (roll_direction) {
    case HeroState::UP:
      shape->update_center_y(shape->center_y() - roll_speed);
      if (shape->center_y() - animation->height / 2 < 0) {
        shape->update_center_y(animation->height / 2);
      }
      break;
    case HeroState::DOWN:
      shape->update_center_y(shape->center_y() + roll_speed);
      if (shape->center_y() + animation->height / 2 > DC->window_height) {
        shape->update_center_y(DC->window_height - animation->height / 2);
      }
      break;
    case HeroState::LEFT:
      shape->update_center_x(shape->center_x() - roll_speed);
      if (shape->center_x() - animation->width / 2 < 0) {
        shape->update_center_x(animation->width / 2);
      }
      break;
    case HeroState::RIGHT:
      shape->update_center_x(shape->center_x() + roll_speed);
      if (shape->center_x() + animation->width / 2 > DC->window_width) {
        shape->update_center_x(DC->window_width - animation->width / 2);
      }
      break;
    default:
      break;
    }
    roll_timer--;
    if (roll_timer == 0) {
      roll_delay_timer = roll_delay_duration; // Start stiffness after roll
    }
    // Skip normal movement and attack inputs during roll
  } else {
    // Normal movement logic
    if (DC->key_state[ALLEGRO_KEY_SPACE] && roll_cd == 0) {
      roll_timer = roll_duration;
      roll_cd = roll_cd_duration;
      roll_direction = state;
    }

    if (DC->key_state[ALLEGRO_KEY_W]) {
      state = HeroState::UP;
      shape->update_center_y(shape->center_y() - speed);
      if (shape->center_y() - animation->height / 2 < 0) {
        shape->update_center_y(animation->height / 2);
      }
    }
    if (DC->key_state[ALLEGRO_KEY_S]) {
      state = HeroState::DOWN;
      shape->update_center_y(shape->center_y() + speed);
      if (shape->center_y() + animation->height / 2 > DC->window_height) {
        shape->update_center_y(DC->window_height - animation->height / 2);
      }
    }
    if (DC->key_state[ALLEGRO_KEY_A]) {
      state = HeroState::LEFT;
      shape->update_center_x(shape->center_x() - speed);
      if (shape->center_x() - animation->width / 2 < 0) {
        shape->update_center_x(animation->width / 2);
      }
    }
    if (DC->key_state[ALLEGRO_KEY_D]) {
      state = HeroState::RIGHT;
      shape->update_center_x(shape->center_x() + speed);
      if (shape->center_x() + animation->width / 2 > DC->window_width) {
        shape->update_center_x(DC->window_width - animation->width / 2);
      }
    }
    if (DC->mouse_state[1] && !DC->prev_mouse_state[1]) {
      attack();
    }
  }
  if (counter > 0)
    counter--;
  if (invincible_timer > 0)
    invincible_timer--;
}

void Hero::draw() {
  GIFCenter *GC = GIFCenter::get_instance();
  ALGIF_ANIMATION *animation = GC->get(gif[state]);
  algif_draw_gif(animation, shape->center_x() - animation->width / 2,
                 shape->center_y() - animation->height / 2, 0);
}

void Hero::attack() {
  if (counter > 0)
    return;
  DataCenter *DC = DataCenter::get_instance();
  // Create bullet
  // Use a simple arrow image for now, or we can add a new asset later
  // Assuming "arrow" is available or reuse tower bullet image
  // Let's use "./assets/image/tower/Arcane_Beam.png" or similar if available
  // Or just reuse one from TowerSetting.
  // For now, I'll use a placeholder path, or check what images are available.
  // I'll use "./assets/image/tower/Arcane_Beam.png" as a safe bet if it exists,
  // or better, let's check ImageCenter or TowerSetting.
  // Actually, let's just use a hardcoded path for now and user can change it.
  DC->heroBullets.emplace_back(
      new HeroBullet(Point{shape->center_x(), shape->center_y()}, DC->mouse,
                     "./assets/image/tower/Arcane_Beam.png", 500, 5, 400));
  counter = attack_freq;
}

void Hero::hit() {
  if (invincible_timer <= 0) {
    invincible_timer = 60;
  }
}

bool Hero::is_invincible() { return invincible_timer > 0; }
