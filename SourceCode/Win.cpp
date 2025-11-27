#include "Win.h"
#include "Utils.h"
#include <allegro5/allegro_primitives.h>

#include "data/DataCenter.h"
#include "data/FontCenter.h"
#include "data/ImageCenter.h"

Win::Win() {}

void Win::init() {
  ImageCenter *IC = ImageCenter::get_instance();
  background = IC->get(win_image_path);
  if (!background) {
    debug_log("Win: Failed to load background image: %s\n", win_image_path);
  }
}

void Win::update() {}

void Win::draw() {
  DataCenter *DC = DataCenter::get_instance();
  FontCenter *FC = FontCenter::get_instance();

  if (background) {
    al_draw_scaled_bitmap(background, 0, 0, al_get_bitmap_width(background),
                          al_get_bitmap_height(background), 0, 0,
                          DC->window_width, DC->window_height, 0);
  }

  al_draw_text(FC->caviar_dreams[FontSize::LARGE], al_map_rgb(255, 255, 255),
               DC->window_width / 2., DC->window_height / 2.,
               ALLEGRO_ALIGN_CENTRE, "YOU WIN");
  al_draw_text(FC->caviar_dreams[FontSize::MEDIUM], al_map_rgb(255, 255, 255),
               DC->window_width / 2., DC->window_height / 2. + 50,
               ALLEGRO_ALIGN_CENTRE, "PRESS ENTER TO MENU");
}
