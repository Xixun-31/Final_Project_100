#ifndef TABLE_H_INCLUDED
#define TABLE_H_INCLUDED

#include "Object.h"
#include "shapes/Rectangle.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

class Table : public Object {
public:
    Table(double x, double y, double w, double h);
    ~Table() {}

    void init();
    void update();
    void draw();
    void toggle();
    
    bool is_active; // State for color
    double w, h;
};

#endif
