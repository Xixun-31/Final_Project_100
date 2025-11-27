#include "Lose.h"
#include "Utils.h"
#include <allegro5/allegro_primitives.h>

constexpr char lose_image_path[] = "./assets/image/scene/LOSE.jpg";

#include "data/DataCenter.h"
#include "data/FontCenter.h"
#include "data/ImageCenter.h"

Lose::Lose() {}

void Lose::init() {
  ImageCenter *IC = ImageCenter::get_instance();
  background = IC->get(lose_image_path);
  if (!background) {
    debug_log("Lose: Failed to load background image: %s\n", lose_image_path);
  }
}

void Lose::update() {}

void Lose::draw() {
  DataCenter *DC = DataCenter::get_instance();
  FontCenter *FC = FontCenter::get_instance();

  if (background) {
    al_draw_scaled_bitmap(background, 0, 0, al_get_bitmap_width(background),
                          al_get_bitmap_height(background), 0, 0,
                          DC->window_width, DC->window_height, 0);
  }

  al_draw_text(FC->caviar_dreams[FontSize::LARGE], al_map_rgb(255, 0, 0),
               DC->window_width / 2., DC->window_height / 2.,
               ALLEGRO_ALIGN_CENTRE, "YOU LOSE");
  al_draw_text(FC->caviar_dreams[FontSize::MEDIUM], al_map_rgb(255, 255, 255),
               DC->window_width / 2., DC->window_height / 2. + 50,
               ALLEGRO_ALIGN_CENTRE, "PRESS ENTER TO MENU");
}
