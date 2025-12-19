#include "Table.h"
#include "Table.h"
#include "data/DataCenter.h"
#include "Table.h"
#include "data/DataCenter.h"
#include "data/FontCenter.h"
#include "data/ImageCenter.h" // Added // Added for text logic

Table::Table(double x, double y, double w, double h) {
    this->w = w;
    this->h = h;
    shape.reset(new Rectangle(x, y, x + w, y + h));
    is_active = false;
    
    ImageCenter *IC = ImageCenter::get_instance();
    img_flipped = IC->get("./assets/image/scene/fliped.png");
    img_unflipped = IC->get("./assets/image/scene/unfilped.png");
}

void Table::init() {
}

void Table::update() {
}

void Table::draw() {
    ALLEGRO_BITMAP *img = is_active ? img_flipped : img_unflipped;
    
    // Draw centered
    // Ensure bitmap exists to avoid crash
    if (img) {
        // Calculate center offset
        // We want to draw the image such that its center aligns with shape->center
        // al_draw_bitmap draws at top-left.
        // So x = center_x - w/2, y = center_y - h/2
        double cx = shape->center_x();
        double cy = shape->center_y();
        double iw = al_get_bitmap_width(img);
        double ih = al_get_bitmap_height(img);
        
        // Scale to match collision box (w, h)
        al_draw_scaled_bitmap(img,
            0, 0, iw, ih, // Source: Full image
            cx - w/2, cy - h/2, w, h, // Dest: Fit to collision box
            0
        );
    } else {
        // Fallback to rectangle if image failed
        ALLEGRO_COLOR color = is_active ? al_map_rgb(255, 0, 0) : al_map_rgb(255, 255, 0);
        double x1 = shape->center_x() - w/2;
        double y1 = shape->center_y() - h/2;
        double x2 = shape->center_x() + w/2;
        double y2 = shape->center_y() + h/2;
        al_draw_filled_rectangle(x1, y1, x2, y2, color);
        al_draw_rectangle(x1, y1, x2, y2, al_map_rgb(0,0,0), 2);
    }
}

void Table::toggle() {
    is_active = !is_active;
}
