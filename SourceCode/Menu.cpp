#include "Menu.h"
#include "Utils.h"
#include "data/DataCenter.h"
#include "data/FontCenter.h"
#include "data/ImageCenter.h"
#include <allegro5/allegro_primitives.h>

constexpr char menu_image_path[] = "./assets/image/scene/Menu.jpg";

Menu::Menu() {}

void Menu::init() {
  ImageCenter *IC = ImageCenter::get_instance();
  menu_img = IC->get(menu_image_path);
}

void Menu::update() {}

void Menu::draw() {
  DataCenter *DC = DataCenter::get_instance();
  FontCenter *FC = FontCenter::get_instance();

  al_draw_scaled_bitmap(menu_img, 0, 0, al_get_bitmap_width(menu_img),
                        al_get_bitmap_height(menu_img), 0, 0, DC->window_width,
                        DC->window_height, 0);
  al_draw_text(FC->caviar_dreams[FontSize::LARGE], al_map_rgb(255, 255, 255),
               DC->window_width / 2., DC->window_height / 2.,
               ALLEGRO_ALIGN_CENTRE, "PRESS SPACE TO START");
}
