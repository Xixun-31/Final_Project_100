#include "MonsterTreasure.h"
#include "../data/ImageCenter.h"
#include "../data/DataCenter.h"
#include "../data/SoundCenter.h"
#include "../Hero.h"
#include <string>
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>

MonsterTreasure::MonsterTreasure(const Point &p) : Monster(MonsterType::TREASURE, p) {
    HP = 1;
    v = 0; // Static
    ImageCenter *IC = ImageCenter::get_instance();
    // Load initial image
    ALLEGRO_BITMAP *bitmap = IC->get("./assets/image/monster/Treasure/treasure_1.png");
    int w = al_get_bitmap_width(bitmap);
    int h = al_get_bitmap_height(bitmap);
    
    // Scale up by 3x
    int scale = 1.5;
    w *= scale;
    h *= scale;

    // Center the treasure at point p
    shape.reset(new Rectangle{p.x - w/2, p.y - h/2, p.x + w/2, p.y + h/2});
    
    dying = false;
    dying_counter = 0;
    bitmap_img_id = 1;
}

void MonsterTreasure::update() {
    if(dying) {
        dying_counter++;
        if(dying_counter >= 15) { // Speed of animation
            dying_counter = 0;
            bitmap_img_id++;
            if(bitmap_img_id > 6) {
                HP = 0;
                DataCenter::get_instance()->hero->power_up();
            }
        }
    } else {
        if(HP <= 0) {
            HP = 1; 
            dying = true;
            bitmap_img_id = 2;
            dying_counter = 0;
            SoundCenter::get_instance()->play("./assets/sound/treasure.WAV", ALLEGRO_PLAYMODE_ONCE);
        }
    }
}

void MonsterTreasure::draw() {
    if(dying && bitmap_img_id > 6) {
        HP = 0; 
        return;
    }

    ImageCenter *IC = ImageCenter::get_instance();
    std::string path = "./assets/image/monster/treasure/treasure_" + std::to_string(bitmap_img_id) + ".png";
    ALLEGRO_BITMAP *bitmap = IC->get(path);
    
    int scale = 3;

    if(bitmap) {
        // Draw scaled 3x
        al_draw_scaled_bitmap(
            bitmap, 
            0, 0, al_get_bitmap_width(bitmap), al_get_bitmap_height(bitmap),
            shape->center_x() - al_get_bitmap_width(bitmap) * scale / 2 + 50, 
            shape->center_y() - al_get_bitmap_height(bitmap) * scale / 2 - 50,
            al_get_bitmap_width(bitmap) * scale, 
            al_get_bitmap_height(bitmap) * scale, 
            0
        );
    }
}
