#include "Table.h"
#include "data/DataCenter.h"

Table::Table(double x, double y, double w, double h) {
    this->w = w;
    this->h = h;
    shape.reset(new Rectangle(x, y, x + w, y + h));
    is_active = false;
}

void Table::init() {
}

void Table::update() {
}

void Table::draw() {
    // Draw filled rectangle
    // Active = Blocking = Red
    // Inactive = Passable = Yellow
    ALLEGRO_COLOR color = is_active ? al_map_rgb(255, 0, 0) : al_map_rgb(255, 255, 0);
    
    // Rectangle: x1, y1, x2, y2
    // Shape stores center. x1 = center_x - w/2
    double x1 = shape->center_x() - w/2;
    double y1 = shape->center_y() - h/2;
    double x2 = shape->center_x() + w/2;
    double y2 = shape->center_y() + h/2;
    
    al_draw_filled_rectangle(x1, y1, x2, y2, color);
    al_draw_rectangle(x1, y1, x2, y2, al_map_rgb(0,0,0), 2);
}

void Table::toggle() {
    is_active = !is_active;
}
