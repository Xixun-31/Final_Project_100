#include "Level3.h"
#include "Utils.h"
#include "data/DataCenter.h"
#include "data/ImageCenter.h"
#include <allegro5/allegro_primitives.h>

constexpr char level3_image_path[] = "./assets/image/scene/Level3.jpg";

Level3::Level3() {}

void Level3::init() {
  ImageCenter *IC = ImageCenter::get_instance();
  background = IC->get(level3_image_path);
}

void Level3::update() {}

void Level3::draw() {
  DataCenter *DC = DataCenter::get_instance();
  al_draw_scaled_bitmap(background, 0, 0, al_get_bitmap_width(background),
                        al_get_bitmap_height(background), 0, 0,
                        DC->window_width, DC->window_height, 0);
}
