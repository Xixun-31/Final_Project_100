#include "Level2.h"
#include "Utils.h"
#include "data/DataCenter.h"
#include "data/ImageCenter.h"
#include <allegro5/allegro_primitives.h>

Level2::Level2() {}

void Level2::init() {
  ImageCenter *IC = ImageCenter::get_instance();
  background = IC->get(level2_image_path);
}

void Level2::update() {}

void Level2::draw() {
  DataCenter *DC = DataCenter::get_instance();
  al_draw_scaled_bitmap(background, 0, 0, al_get_bitmap_width(background),
                        al_get_bitmap_height(background), 0, 0,
                        DC->window_width, DC->window_height, 0);
}
