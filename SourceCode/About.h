#ifndef ABOUT_H_INCLUDED
#define ABOUT_H_INCLUDED

#include <allegro5/allegro.h>
#include "shapes/Rectangle.h"

class About {
public:
	About();
	void init();
	void update();
	void draw();

	int hover_btn; // 0: None, 1: Menu, 2: God Mode
private:
	ALLEGRO_BITMAP *about_img;
	Rectangle menu_btn;
	Rectangle god_mode_btn;
};

#endif
