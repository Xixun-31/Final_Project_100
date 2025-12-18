#include "MonsterCaveMan.h"
#include "../data/DataCenter.h"
#include "../data/ImageCenter.h"
#include "../towers/Bullet.h"
#include <allegro5/allegro_primitives.h>

void MonsterCaveMan::update() {
    double now = al_get_time();

    if (is_shooting) {
        
        if (now - shoot_anim_start >= 3 * shoot_frame_duration) is_shooting = false;
    return;
  }
    Monster::update();
}

void MonsterCaveMan::draw() {
    if (is_shooting) {
        ImageCenter* IC = ImageCenter::get_instance();
        double now = al_get_time();
        double t = now - shoot_anim_start;
        // 算現在是第幾幀
        int frame = static_cast<int>(t / shoot_frame_duration);

        if (frame >= shoot_frame_count) {
            // 動畫播完，就結束射擊狀態，回到走路動畫
            is_shooting = false;
            Monster::draw();
            return;
        }
        // 根據 frame 決定貼圖路徑
        std::string path =
            "./assets/image/monster/CaveMan/SHOOT_" + std::to_string(frame) + ".png";

        ALLEGRO_BITMAP* bmp = IC->get(path);

        al_draw_bitmap(
            bmp,
            shape->center_x() - al_get_bitmap_width(bmp) / 2,
            shape->center_y() - al_get_bitmap_height(bmp) / 2,
            0
        );
        return;
    }
    Monster::draw();
}

void MonsterCaveMan::special_ability(DataCenter* DC) {
    if (HP <= 0) return;  // 死了就不要射
  
    double now = al_get_time();

    // if shooting 
    if (is_shooting) {
      if (now - shoot_anim_start > 0.3) {
        is_shooting = false;
      }
    }

    // 2. 檢查冷卻：沒冷卻好就不射
    if (now - last_shoot_time < shoot_cooldown)
      return;

    // 3. 冷卻好了 → 開槍！
    last_shoot_time = now;
    is_shooting = true;
    shoot_anim_start = now;

    // 3-1. 算出從怪物中心指向 hero 的方向
    Point from{ shape->center_x(), shape->center_y() };
    Point to  { DC->hero->shape->center_x(), DC->hero->shape->center_y() };

    double speed = 300.0;     // 子彈速度
    int dmg      = 3;         // 傷害
    double range = 2500.0;     // 射程 (Increased to cover map)

    // Bullet 只需要一個「方向」用的 target，我們可以用 hero 位置就好
    Bullet* b = new Bullet(
      from,
      to,
      "assets/image/tower/Storm_Beam.png",  
      speed,
      dmg,
      range
    );

    DC->monsterBullets.push_back(b);
}