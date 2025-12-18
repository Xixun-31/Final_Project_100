#include "About.h"
#include "Utils.h"
#include "data/DataCenter.h"
#include "data/FontCenter.h"
#include "data/ImageCenter.h"
#include "Player.h" // Added for God Mode usage
#include <allegro5/allegro_primitives.h>

// You might need a specific background for About, reusing Menu or StartBackground for now if not specified.
// However, the user asked for a "new screen". I'll use a placeholder or reuse an existing image for now 
// and draw text over it, or assume there's an asset. 
// Since I don't have a specific "About.jpg", I'll reuse "Menu.jpg" or just a colored background with text.
// Let's use Menu.jpg as base and overlay text.

constexpr char about_image_path[] = "./assets/image/scene/Menu.jpg"; 

About::About() {
	hover_btn = 0;
}

void About::init() {
	ImageCenter *IC = ImageCenter::get_instance();
	about_img = IC->get(about_image_path);
	DataCenter *DC = DataCenter::get_instance();
	
	int w = 200;
	int h = 60;
	int cx = DC->window_width / 2;
	int cy = 500;

	menu_btn = Rectangle(cx - w/2, cy, cx + w/2, cy + h);
	// God Mode button on top right
	god_mode_btn = Rectangle(DC->window_width - 220, 20, DC->window_width - 20, 60);
}

void About::update() {
	DataCenter *DC = DataCenter::get_instance();
	Point mouse = DC->mouse;
	
	hover_btn = 0;
	if(menu_btn.overlap(mouse)) hover_btn = 1;
	else if(god_mode_btn.overlap(mouse)) hover_btn = 2;
}

void About::draw() {
	DataCenter *DC = DataCenter::get_instance();
	FontCenter *FC = FontCenter::get_instance();

	// Draw background
	al_draw_scaled_bitmap(about_img, 0, 0, al_get_bitmap_width(about_img),
						  al_get_bitmap_height(about_img), 0, 0, DC->window_width,
						  DC->window_height, 0);

	// Draw "About" text
	al_draw_text(FC->consolas[FontSize::LARGE], al_map_rgb(255, 255, 255),
				 DC->window_width / 2., 100,
				 ALLEGRO_ALIGN_CENTRE, "HOW TO PLAY");

	al_draw_text(FC->consolas[FontSize::MEDIUM], al_map_rgb(255, 255, 255),
				 DC->window_width / 2., 200,
				 ALLEGRO_ALIGN_CENTRE, "W, A, S, D to Move");
	
	al_draw_text(FC->consolas[FontSize::MEDIUM], al_map_rgb(255, 255, 255),
				 DC->window_width / 2., 250,
				 ALLEGRO_ALIGN_CENTRE, "Mouse Left Click to Attack");

	al_draw_text(FC->consolas[FontSize::MEDIUM], al_map_rgb(255, 255, 255),
				 DC->window_width / 2., 300,
				 ALLEGRO_ALIGN_CENTRE, "Space to Slide (Invincible but Stiff)");

	al_draw_text(FC->consolas[FontSize::MEDIUM], al_map_rgb(255, 255, 255),
				 DC->window_width / 2., 350,
				 ALLEGRO_ALIGN_CENTRE, "Mouse Right Click to Clear Bullets");

	// Helper
	auto draw_btn = [&](Rectangle &r, const char* text, bool hover) {
		ALLEGRO_COLOR color = hover ? al_map_rgb(173, 216, 230) : al_map_rgb(255, 255, 255);
		al_draw_text(FC->consolas[FontSize::MEDIUM], color,
			(r.x1 + r.x2) / 2, r.y1 + 10,
			ALLEGRO_ALIGN_CENTRE, text);
	};
	// Fixed duplicates
	draw_btn(menu_btn, "MENU", hover_btn == 1);
	
	// Draw God Mode button
	bool is_god = DC->player->is_god_mode;
	ALLEGRO_COLOR god_btn_color = al_map_rgb(255, 255, 255);
	if(hover_btn == 2) god_btn_color = al_map_rgb(173, 216, 230); // Hover also light blue
	
	// If ON, maybe stronger indication? Or just text.
	// "INVINCIBLE"
	const char* god_text = is_god ? "INVINCIBLE: ON" : "INVINCIBLE: OFF";
	al_draw_text(FC->consolas[FontSize::MEDIUM], god_btn_color,
			(god_mode_btn.x1 + god_mode_btn.x2) / 2, god_mode_btn.y1 + 10,
			ALLEGRO_ALIGN_CENTRE, god_text);
}
