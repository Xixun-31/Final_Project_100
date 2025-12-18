#include "Lose.h"
#include "Utils.h"
#include <allegro5/allegro_primitives.h>

constexpr char lose_image_path[] = "./assets/image/scene/LOSE.jpg";

#include "data/DataCenter.h"
#include "data/FontCenter.h"
#include "data/ImageCenter.h"

Lose::Lose() {
	hover_btn = 0;
}

void Lose::init() {
  ImageCenter *IC = ImageCenter::get_instance();
  background = IC->get(lose_image_path);
  if (!background) {
    debug_log("Lose: Failed to load background image: %s\n", lose_image_path);
  }

  DataCenter *DC = DataCenter::get_instance();
  // Centered buttons below text
  int w = 200;
  int h = 60;
  // Adjusted Y to not overlap
  int cx = DC->window_width / 2;
  int cy = DC->window_height / 2 + 100;

  menu_btn = Rectangle(cx - w - 20, cy, cx - 20, cy + h);
  exit_btn = Rectangle(cx + 20, cy, cx + w + 20, cy + h);
}

void Lose::update() {
	DataCenter *DC = DataCenter::get_instance();
	Point mouse = DC->mouse;
	
	hover_btn = 0;
	if(menu_btn.overlap(mouse)) hover_btn = 1;
	else if(exit_btn.overlap(mouse)) hover_btn = 2;
}

void Lose::draw() {
  DataCenter *DC = DataCenter::get_instance();
  FontCenter *FC = FontCenter::get_instance();

  if (background) {
    al_draw_scaled_bitmap(background, 0, 0, al_get_bitmap_width(background),
                          al_get_bitmap_height(background), 0, 0,
                          DC->window_width, DC->window_height, 0);
  }

  al_draw_text(FC->consolas[FontSize::EXTRA_LARGE], al_map_rgb(255, 0, 0),
               DC->window_width / 2., DC->window_height / 2. - 50,
               ALLEGRO_ALIGN_CENTRE, "YOU LOSE");
  
  // Helper
  auto draw_btn = [&](Rectangle &r, const char* text, bool hover) {
      ALLEGRO_COLOR color = hover ? al_map_rgb(173, 216, 230) : al_map_rgb(255, 255, 255);
      // al_draw_rectangle(r.x1, r.y1, r.x2, r.y2, color, 2);
      al_draw_text(FC->consolas[FontSize::MEDIUM], color,
          (r.x1 + r.x2) / 2, r.y1 + 10,
          ALLEGRO_ALIGN_CENTRE, text);
  };

  draw_btn(menu_btn, "MENU", hover_btn == 1);
  draw_btn(exit_btn, "EXIT", hover_btn == 2);
}
