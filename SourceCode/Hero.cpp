#include "Hero.h"
#include "HeroBullet.h"
#include "Player.h"
#include "algif5/algif.h"
#include "data/DataCenter.h"
#include "data/ImageCenter.h"
#include "shapes/Rectangle.h"
#include <allegro5/allegro_primitives.h>
#include <cstring>

namespace HeroSetting {
static constexpr char Hero_images_root_path[40] = "./assets/image/hero/stable_";
static constexpr char dir_path_postfix[][10] = {"left", "right", "back",
                                                "front"};
} // namespace HeroSetting

void Hero::init() {
  img.clear();
  run_img.clear();
  shoot_img.clear();
  dead_img.clear();

  for (int i = 0; i < static_cast<int>(HeroState::HeroState_MAX); ++i) {
    img[static_cast<HeroState>(i)] =
        std::string(HeroSetting::Hero_images_root_path) +
        HeroSetting::dir_path_postfix[i] + ".png";

    // Load running images
    std::string dir_prefix;
    switch (static_cast<HeroState>(i)) {
    case HeroState::LEFT:
      dir_prefix = "left";
      break;
    case HeroState::RIGHT:
      dir_prefix = "right";
      break;
    case HeroState::UP:
      dir_prefix = "up";
      break;
    case HeroState::DOWN:
      dir_prefix = "down";
      break;
    default:
      dir_prefix = "down";
      break;
    }

    for (int j = 1; j <= 4; ++j) {
      run_img[static_cast<HeroState>(i)].push_back(
          "./assets/image/hero/" + dir_prefix + std::to_string(j) + ".png");
    }
  }

  // Load special action images
  for (int j = 1; j <= 3; ++j) {
    shoot_img.push_back("./assets/image/hero/shoot" + std::to_string(j) +
                        ".png");
  }
  for (int j = 1; j <= 5; ++j) {
    dead_img.push_back("./assets/image/hero/dead" + std::to_string(j) + ".png");
  }
  slide_img = "./assets/image/hero/slide.png";
  pain_img = "./assets/image/hero/pain.png";
  delay_img = "./assets/image/hero/delay.png";
  prepare_img.clear();
  boom_img.clear();
  boom_img.push_back("./assets/image/hero/boom1.png");
  boom_img.push_back("./assets/image/hero/boom2.png");

  // Reset state variables
  is_moving = false;
  is_attacking = false;
  is_dying = false;
  is_pain = false;
  is_reloading = false;
  is_bombing = false;
  bomb_timer = 0;
  bomb_count = max_bomb_count;
  ammo = max_ammo;
  reload_timer = 0;
  animation_frame = 0;
  animation_tick = 0;
  attack_anim_timer = 0;
  death_anim_timer = 0;
  state = HeroState::UP;
  counter = 0;
  invincible_timer = 0;
  roll_timer = 0;
  roll_cd = 0;
  roll_delay_timer = 0;

  ImageCenter *IC = ImageCenter::get_instance();
  ALLEGRO_BITMAP *bitmap = IC->get(img[state]);
  if (bitmap) {
    DataCenter *DC = DataCenter::get_instance();
    shape.reset(
        new Rectangle{DC->window_height / 2.0, DC->window_width / 2.0,
                      DC->window_height / 2.0 + al_get_bitmap_width(bitmap) / 2,
                      DC->window_width / 2.0 + al_get_bitmap_height(bitmap)});
  }
}

void Hero::update() {
  DataCenter *DC = DataCenter::get_instance();
  if (roll_cd > 0)
    roll_cd--;

  if (is_reloading) {
    reload_timer--;
    if (reload_timer <= 0) {
      is_reloading = false;
      ammo = max_ammo;
    }
    // Allow movement but stop other updates like attack
  }

  // Manual reload
  if (DC->key_state[ALLEGRO_KEY_R] && !DC->prev_key_state[ALLEGRO_KEY_R] &&
      ammo < max_ammo) {
    reload();
    // Don't return, allow movement
  }

  // Bomb activation
  if (DC->key_state[ALLEGRO_KEY_B] && !DC->prev_key_state[ALLEGRO_KEY_B]) {
    activate_bomb();
  }

  if (is_bombing) {
    bomb_timer--;
    if (bomb_timer <= 0) {
      is_bombing = false;
    }
  }

  if (invincible_timer > 0) {
    invincible_timer--;
    if (invincible_timer == 0) {
      is_pain = false;
    }
  }

  // Death animation logic (check HP)
  if (DC->player->HP <= 0 && !is_dying) {
    is_dying = true;
    animation_frame = 0;
    death_anim_timer = 0;
  }

  if (is_dying) {
    death_anim_timer++;
    if (animation_frame < 4) {
      if (death_anim_timer >= 20) { // Slower animation (20 ticks per frame)
        animation_frame++;
        death_anim_timer = 0;
      }
    } else {
      // Last frame, wait longer before finishing
      if (death_anim_timer >= 60) { // Wait 60 ticks (1 second) at the end
        // Animation done, waiting for Game to switch state
      }
    }
    return; // Stop other updates
  }

  if (counter > 0)
    counter--;

  // Normal movement logic
  if (DC->key_state[ALLEGRO_KEY_SPACE] && roll_cd == 0) {
    roll_timer = roll_duration;
    roll_cd = roll_cd_duration;
    roll_direction = state;
  }

  ImageCenter *IC = ImageCenter::get_instance();
  ALLEGRO_BITMAP *bitmap = IC->get(img[state]);
  int w = al_get_bitmap_width(bitmap);
  int h = al_get_bitmap_height(bitmap);

  is_moving = false;

  // Roll logic
  if (roll_timer > 0) {
    roll_timer--;
    invincible_timer = roll_delay_duration; // Keep invincible during roll

    switch (roll_direction) {
    case HeroState::UP:
      shape->update_center_y(shape->center_y() - roll_speed);
      if (shape->center_y() - h / 2 < 0) {
        shape->update_center_y(h / 2);
      }
      break;
    case HeroState::DOWN:
      shape->update_center_y(shape->center_y() + roll_speed);
      if (shape->center_y() + h / 2 > DC->window_height) {
        shape->update_center_y(DC->window_height - h / 2);
      }
      break;
    case HeroState::LEFT:
      shape->update_center_x(shape->center_x() - roll_speed);
      if (shape->center_x() - w / 4 < 0) {
        shape->update_center_x(w / 4);
      }
      break;
    case HeroState::RIGHT:
      shape->update_center_x(shape->center_x() + roll_speed);
      if (shape->center_x() + w / 4 > DC->window_width) {
        shape->update_center_x(DC->window_width - w / 4);
      }
      break;
    default:
      break;
    }

    if (roll_timer == 0) {
      DC->hero->roll_delay_timer = DC->hero->roll_delay_duration;
    }
  } else if (roll_delay_timer > 0) {
    roll_delay_timer--;
  } else {
    // Normal movement
    if (DC->key_state[ALLEGRO_KEY_W]) {
      state = HeroState::UP;
      is_moving = true;
      shape->update_center_y(shape->center_y() - speed);
      if (shape->center_y() - h / 2 < 0) {
        shape->update_center_y(h / 2);
      }
    }
    if (DC->key_state[ALLEGRO_KEY_S]) {
      state = HeroState::DOWN;
      is_moving = true;
      shape->update_center_y(shape->center_y() + speed);
      if (shape->center_y() + h / 2 > DC->window_height) {
        shape->update_center_y(DC->window_height - h / 2);
      }
    }
    if (DC->key_state[ALLEGRO_KEY_A]) {
      state = HeroState::LEFT;
      is_moving = true;
      shape->update_center_x(shape->center_x() - speed);
      if (shape->center_x() - w / 4 < 0) {
        shape->update_center_x(w / 4);
      }
    }
    if (DC->key_state[ALLEGRO_KEY_D]) {
      state = HeroState::RIGHT;
      is_moving = true;
      shape->update_center_x(shape->center_x() + speed);
      if (shape->center_x() + w / 4 > DC->window_width) {
        shape->update_center_x(DC->window_width - w / 4);
      }
    }

    // Animation update
    if (is_attacking) {
      attack_anim_timer++;
      if (attack_anim_timer >= 5) {
        animation_frame++;
        attack_anim_timer = 0;
        if (animation_frame >= 3) {
          is_attacking = false;
          animation_frame = 0;
        }
      }
    } else if (is_moving) {
      animation_tick++;
      if (animation_tick >= 10) {
        animation_frame = (animation_frame + 1) % 4;
        animation_tick = 0;
      }
    } else {
      animation_frame = 0;
      animation_tick = 0;
    }

    if (DC->mouse_state[1] && !DC->prev_mouse_state[1]) {
      attack();
    }
  }
}

void Hero::draw() {
  ImageCenter *IC = ImageCenter::get_instance();
  ALLEGRO_BITMAP *bitmap;

  if (is_dying) {
    bitmap = IC->get(dead_img[animation_frame]);
  } else if (is_reloading) {
    // Draw normal movement/idle image during reload
    if (is_moving) {
      bitmap = IC->get(run_img[state][animation_frame]);
    } else {
      bitmap = IC->get(img[state]);
    }

    // Draw reload bar
    float ratio = (float)reload_timer / reload_duration;
    float bar_width = 40;
    float bar_height = 5;
    float x = shape->center_x() - bar_width / 2;
    float y = shape->center_y() - 50;
    al_draw_filled_rectangle(x, y, x + bar_width * ratio, y + bar_height,
                             al_map_rgb(255, 255, 255));
  } else if (is_bombing) {
    int frame_idx = (bomb_duration - bomb_timer) / 5 % 2;
    bitmap = IC->get(boom_img[frame_idx]);

    // Draw expanding circles (shockwave)
    DataCenter *DC = DataCenter::get_instance();
    float max_radius = std::max(DC->window_width, DC->window_height);
    float progress = (float)(bomb_duration - bomb_timer) / bomb_duration;
    float radius1 = progress * max_radius * 1.5; // Faster and larger
    float radius2 = (progress - 0.1) * max_radius * 1.5;

    if (radius1 > 0)
      al_draw_circle(shape->center_x(), shape->center_y(), radius1,
                     al_map_rgb(255, 0, 0), 5);
    if (radius2 > 0)
      al_draw_circle(shape->center_x(), shape->center_y(), radius2,
                     al_map_rgb(255, 100, 100), 5);
  } else if (roll_timer > 0) {
    bitmap = IC->get(slide_img);
  } else if (is_pain) {
    bitmap = IC->get(pain_img);
  } else if (is_attacking) {
    bitmap = IC->get(shoot_img[animation_frame]);
  } else if (is_moving) {
    bitmap = IC->get(run_img[state][animation_frame]);
  } else {
    bitmap = IC->get(img[state]);
  }

  if (bitmap == nullptr)
    return;

  al_draw_scaled_bitmap(
      bitmap, 0, 0, al_get_bitmap_width(bitmap), al_get_bitmap_height(bitmap),
      shape->center_x() - al_get_bitmap_width(bitmap) / 2,
      shape->center_y() - al_get_bitmap_height(bitmap) / 2,
      al_get_bitmap_width(bitmap), al_get_bitmap_height(bitmap), 0);
}

void Hero::attack() {
  if (is_attacking || counter > 0 || is_reloading)
    return;
  if (ammo <= 0) {
    reload();
    return;
  }
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

  ammo--;
  counter = attack_freq;
  is_attacking = true;
  animation_frame = 0;
  attack_anim_timer = 0;
}

void Hero::reload() {
  if (is_reloading)
    return;
  is_reloading = true;
  reload_timer = reload_duration;
}

void Hero::activate_bomb() {
  if (is_bombing || bomb_count <= 0)
    return;
  is_bombing = true;
  bomb_timer = bomb_duration;
  bomb_count--;

  // Clear all bullets
  DataCenter *DC = DataCenter::get_instance();
  for (auto bullet : DC->towerBullets) {
    delete bullet;
  }
  DC->towerBullets.clear();
}

void Hero::hit() {
  DataCenter *DC = DataCenter::get_instance();
  if (invincible_timer <= 0) {
    DC->player->HP--;
    invincible_timer = 30; // Invincible for 30 frames
    is_pain = true;
  }
}

bool Hero::is_invincible() { return invincible_timer > 0; }
