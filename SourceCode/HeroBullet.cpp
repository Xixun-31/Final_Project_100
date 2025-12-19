#include "HeroBullet.h"
#include "Utils.h"
#include "data/DataCenter.h"
#include "data/ImageCenter.h"
#include "monsters/Monster.h"
#include "shapes/Rectangle.h"
#include <allegro5/bitmap_draw.h>

HeroBullet::HeroBullet(const Point &p, const Point &target,
                       const std::string &path, double v, int dmg,
                       double fly_dist, bool animated)
    : Bullet(p, target, path, v, dmg, fly_dist) {
    is_animated = animated;
    anim_timer = 0;
    if(animated) {
        ImageCenter *IC = ImageCenter::get_instance();
        // Assume path is one of them, load the other.
        // Or just load both explicitly to be safe.
        bmp1 = IC->get("./assets/image/hero/bullet_1.png");
        bmp2 = IC->get("./assets/image/hero/bullet_2.png");
        // Ensure initial bitmap is set correctly (Bullet sets it to 'path')
        // We'll let it stay as is for start.
    } else {
        bmp1 = nullptr;
        bmp2 = nullptr;
    }

    // Resize shape if animated (upgraded)
    double scale = 1.0;
    if(is_animated) scale = 1.5; // Upgraded bullet 1.5x

    if(bitmap) {
        double r = std::min(al_get_bitmap_width(bitmap), al_get_bitmap_height(bitmap)) * 0.8;
        shape.reset(new Circle{p.x, p.y, r * scale}); 
    }
}

void HeroBullet::update() {
  Bullet::update(); // Move the bullet
  
  if(is_animated && bmp1 && bmp2) {
      anim_timer++;
      if(anim_timer >= 15) { // Switch every 15 frames
          anim_timer = 0;
          if(bitmap == bmp1) {
              bitmap = bmp2;
          } else {
              bitmap = bmp1;
          }
      }
  }
}

void HeroBullet::draw() {
    if(!bitmap) return;
    
    double scale = 1.0;
    if(is_animated) scale = 1.5;

    // Draw scaled
    al_draw_scaled_bitmap(
        bitmap,
        0, 0, al_get_bitmap_width(bitmap), al_get_bitmap_height(bitmap),
        shape->center_x() - al_get_bitmap_width(bitmap) * scale / 2, 
        shape->center_y() - al_get_bitmap_height(bitmap) * scale / 2,
        al_get_bitmap_width(bitmap) * scale,
        al_get_bitmap_height(bitmap) * scale,
        0
    );
}
