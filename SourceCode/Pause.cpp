#include "Pause.h"
#include "data/DataCenter.h"
#include "data/FontCenter.h"
#include <allegro5/allegro_primitives.h>

Pause::Pause() {
    hover_btn = 0;
}

void Pause::init() {
    DataCenter *DC = DataCenter::get_instance();
    int w = 200;
    int h = 60;
    int cx = DC->window_width / 2;
    int cy = DC->window_height / 2;

    // Center the buttons, starting a bit lower than center to leave room for "GAME PAUSED" text
    resume_btn = Rectangle(cx - w/2, cy - h - 10, cx + w/2, cy - 10);
    menu_btn = Rectangle(cx - w/2, cy + 10, cx + w/2, cy + h + 10);
    exit_btn = Rectangle(cx - w/2, cy + h + 30, cx + w/2, cy + h + h + 30);
}

void Pause::update() {
    DataCenter *DC = DataCenter::get_instance();
    Point mouse = DC->mouse;

    hover_btn = 0;
    if (resume_btn.overlap(mouse)) hover_btn = 1;
    else if (menu_btn.overlap(mouse)) hover_btn = 2;
    else if (exit_btn.overlap(mouse)) hover_btn = 3;
}

void Pause::draw() {
    DataCenter *DC = DataCenter::get_instance();
    FontCenter *FC = FontCenter::get_instance();

    // Semi-transparent background
    al_draw_filled_rectangle(0, 0, DC->window_width, DC->window_height,
                             al_map_rgba(50, 50, 50, 192));

    // Title
    al_draw_text(FC->consolas[FontSize::LARGE], al_map_rgb(255, 255, 255),
                 DC->window_width / 2., DC->window_height / 2. - 150,
                 ALLEGRO_ALIGN_CENTRE, "GAME PAUSED");

    // Helper lambda for drawing buttons
    auto draw_btn = [&](Rectangle &r, const char* text, bool hover) {
        // Change text color based on hover (Light Blue vs White)
        // Light Blue: 173, 216, 230
        ALLEGRO_COLOR color = hover ? al_map_rgb(173, 216, 230) : al_map_rgb(255, 255, 255);

        // Draw text centered (smaller font, no outline)
        al_draw_text(FC->consolas[FontSize::MEDIUM], color,
            (r.x1 + r.x2) / 2, r.y1 + 15,
            ALLEGRO_ALIGN_CENTRE, text);
    };

    draw_btn(resume_btn, "RESUME", hover_btn == 1);
    draw_btn(menu_btn, "MENU", hover_btn == 2);
    draw_btn(exit_btn, "EXIT", hover_btn == 3);
}
