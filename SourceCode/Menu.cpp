#include "Menu.h"
#include "Utils.h"
#include "data/DataCenter.h"
#include "data/FontCenter.h"
#include "data/ImageCenter.h"
#include <allegro5/allegro_primitives.h>

constexpr char menu_image_path[] = "./assets/image/scene/Menu.jpg";

Menu::Menu() {
	hover_btn = 0;
}

void Menu::init() {
  ImageCenter *IC = ImageCenter::get_instance();
  menu_img = IC->get(menu_image_path);

  DataCenter *DC = DataCenter::get_instance();
  // Centered buttons
  int w = 200;
  int h = 60;
  int cx = DC->window_width / 2;
  int cy = DC->window_height / 2;

  start_btn = Rectangle(cx - w/2, cy, cx + w/2, cy + h);
  about_btn = Rectangle(cx - w/2, cy + h + 20, cx + w/2, cy + h + h + 20);
  exit_btn = Rectangle(cx - w/2, cy + h + h + 40, cx + w/2, cy + h + h + h + 40);
}

void Menu::update() {
	DataCenter *DC = DataCenter::get_instance();
	Point mouse = DC->mouse;
	
	hover_btn = 0;
	if(start_btn.overlap(mouse)) hover_btn = 1;
	else if(about_btn.overlap(mouse)) hover_btn = 2;
	else if(exit_btn.overlap(mouse)) hover_btn = 3;
}

void Menu::draw() {
  DataCenter *DC = DataCenter::get_instance();
  FontCenter *FC = FontCenter::get_instance();

  al_draw_scaled_bitmap(menu_img, 0, 0, al_get_bitmap_width(menu_img),
                        al_get_bitmap_height(menu_img), 0, 0, DC->window_width,
                        DC->window_height, 0);

  // Title
  al_draw_text(FC->consolas[FontSize::EXTRA_LARGE], al_map_rgb(255, 255, 255),
               DC->window_width / 2., 150,
               ALLEGRO_ALIGN_CENTRE, "ENTER THE GUNGEON 2");

  // Helper lambda for drawing buttons
  auto draw_btn = [&](Rectangle &r, const char* text, bool hover) {
      // Change color based on hover
      ALLEGRO_COLOR color = hover ? al_map_rgb(173, 216, 230) : al_map_rgb(255, 255, 255);
      
      // Draw text centered (no outline)
      al_draw_text(FC->consolas[FontSize::LARGE], color,
          (r.x1 + r.x2) / 2, r.y1 + 10,
          ALLEGRO_ALIGN_CENTRE, text);
  };

  draw_btn(start_btn, "START", hover_btn == 1);
  draw_btn(about_btn, "ABOUT", hover_btn == 2);
  draw_btn(exit_btn, "EXIT", hover_btn == 3);
}
