#include "MonsterBarrel.h"
#include "../data/DataCenter.h"
#include "../data/ImageCenter.h"
#include "../Effect.h"
#include "../shapes/Circle.h"
#include "../Hero.h"
#include <string>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

MonsterBarrel::MonsterBarrel(const Point &p) : Monster(MonsterType::BARREL, p) {
    HP = 1;
    v = 0; // Does not move
    exploded = false;
    
    // Load animation frames
    // "barrel_1" and "barrel_2"
    // Using existing bitmap_img_ids vector from Base Monster but defining paths here or in init
    // Better just load manually or use ImageCenter directly in draw or setup path vector
    // Base Monster class usually handles path via "assets/image/monster/TYPE/DIR.png" convention or similar.
    // Let's check MonsterCaveMan for example. 
    // It seems Monster base class expects specific paths. 
    // I'll override draw to handle specific barrel animation simpler.
}

void MonsterBarrel::update() {
    if(exploded) return;
    
    // Animation toggle: 0.2s interval (12 frames @ 60 FPS)
    // bitmap_switch_counter handled in base? or custom?
    // Let's implement custom simple counter
    bitmap_switch_counter++;
    if(bitmap_switch_counter >= 12) { // 0.2s approx
        bitmap_switch_counter = 0;
        bitmap_img_id = (bitmap_img_id == 1) ? 2 : 1;
    }
    
    if(HP <= 0 && !exploded) {
        exploded = true;
        
        // Explode!
        Effect::emit_explosion(Point{shape->center_x(), shape->center_y()});
        
        DataCenter *DC = DataCenter::get_instance();
        // Damage Radius: let's say 150
        double radius = 150.0;
        Circle explosion_area(shape->center_x(), shape->center_y(), radius);
        
        // Damage Hero
        if(DC->hero->shape->overlap(explosion_area)) {
            DC->hero->hit(); // Deals 1 dmg typically
        }
        
        // Damage Monsters
        for(Monster *m : DC->monsters) {
            if(m == this) continue;
            if(m->shape->overlap(explosion_area)) {
                m->HP -= 1; 
                // Don't modify vector while iterating? 
                // We are just modifying HP, not list structure, so it's safe. 
                // Removal happens in Game loop.
            }
        }
    }
}

void MonsterBarrel::draw() {
    if(exploded) return;
    
    ImageCenter *IC = ImageCenter::get_instance();
    std::string path = "./assets/image/monster/barrel/barrel_" + std::to_string(bitmap_img_id > 0 ? bitmap_img_id : 1) + ".png";
    ALLEGRO_BITMAP *bitmap = IC->get(path);
    
    if(bitmap) {
         al_draw_bitmap(bitmap, shape->center_x() - al_get_bitmap_width(bitmap)/2, 
                        shape->center_y() - al_get_bitmap_height(bitmap)/2, 0);
    } else {
        // Fallback
        al_draw_filled_circle(shape->center_x(), shape->center_y(), 20, al_map_rgb(139, 69, 19));
    }
}
