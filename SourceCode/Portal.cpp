#include "Portal.h"
#include "data/DataCenter.h"
#include "data/ImageCenter.h"
#include "shapes/Rectangle.h"
#include <allegro5/allegro_primitives.h>

Portal::Portal(double x, double y) {
    img_path = "./assets/image/circle.png"; 
    
    // Fixed small size: 60x60 (Radius 30)
    int w = 60; 
    int h = 60;
    
    shape.reset(new Rectangle(x - w/2, y - h/2, x + w/2, y + h/2));
}

void Portal::init() {
}

void Portal::update() {
}

void Portal::draw() {
    ImageCenter *IC = ImageCenter::get_instance();
    ALLEGRO_BITMAP *bitmap = IC->get(img_path);
    
    // No purple circle
    
    if(bitmap) {
        // Draw scaled to 60x60
        al_draw_scaled_bitmap(
            bitmap, 
            0, 0, al_get_bitmap_width(bitmap), al_get_bitmap_height(bitmap), // Source
            shape->center_x() - 30, shape->center_y() - 30, // Dest X, Y
            60, 60, // Dest W, H
            0 // Flags
        );
    } else {
        // Simple fallback if image missing (Blue circle outline maybe? Or just nothing as requested? 
        // User said "don't want purple in middle". 
        // I'll draw a white circle outline just in case so it's not invisible.
        al_draw_circle(shape->center_x(), shape->center_y(), 30, al_map_rgb(255, 255, 255), 2);
    }
}
