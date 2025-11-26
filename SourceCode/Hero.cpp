#include "Hero.h"
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
  if (DC->key_state[ALLEGRO_KEY_W]) {
    state = HeroState::UP;
    shape->update_center_y(shape->center_y() - speed);
  }
  if (DC->key_state[ALLEGRO_KEY_S]) {
    state = HeroState::DOWN;
    shape->update_center_y(shape->center_y() + speed);
  }
  if (DC->key_state[ALLEGRO_KEY_A]) {
    state = HeroState::LEFT;
    shape->update_center_x(shape->center_x() - speed);
  }
  if (DC->key_state[ALLEGRO_KEY_D]) {
    state = HeroState::RIGHT;
    shape->update_center_x(shape->center_x() + speed);
  }
}

void Hero::draw() {
  GIFCenter *GC = GIFCenter::get_instance();
  ALGIF_ANIMATION *animation = GC->get(gif[state]);
  algif_draw_gif(animation, shape->center_x() - animation->width / 2,
                 shape->center_y() - animation->height / 2, 0);
}
