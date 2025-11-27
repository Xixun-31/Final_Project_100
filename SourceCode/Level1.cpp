#include "Level1.h"
#include "Utils.h"
#include "data/DataCenter.h"
#include "data/ImageCenter.h"
#include <allegro5/allegro_primitives.h>

Level1::Level1() {}

void Level1::init() {
  ImageCenter *IC = ImageCenter::get_instance();
  background = IC->get(level1_image_path);
}

void Level1::update() {}

void Level1::draw() {
  DataCenter *DC = DataCenter::get_instance();
  al_draw_scaled_bitmap(background, 0, 0, al_get_bitmap_width(background),
                        al_get_bitmap_height(background), 0, 0,
                        DC->window_width, DC->window_height, 0);
}
