#include "MonsterElite.h"
#include "../data/DataCenter.h"
#include "../data/ImageCenter.h"
#include "../towers/Bullet.h"

#include <cmath>

MonsterElite::MonsterElite(const Point& p)
    : Monster(MonsterType::ELITE, p)   // 記得在你的 MonsterType enum 加一個 ELITE
{
    // 基本能力值你可以自己調
    HP    = 250;
    v     = 40;   // 一般移動速度（MOVE 用）
    money = 20;

    // 設定 idle / move 動畫圖片路徑
    idle_img = {
        "./assets/image/monster/Elite/STILL_0.png",
        "./assets/image/monster/Elite/STILL_1.png",
        "./assets/image/monster/Elite/STILL_2.png",
        "./assets/image/monster/Elite/STILL_3.png"
    };

    move_img = {
        "./assets/image/monster/Elite/MOVE_0.png",
        "./assets/image/monster/Elite/MOVE_1.png",
        "./assets/image/monster/Elite/MOVE_2.png",
        "./assets/image/monster/Elite/MOVE_3.png",
        "./assets/image/monster/Elite/MOVE_4.png"
    };

    diag_bullet = {
        "./assets/image/monster/Elite/BULLET_UL.png",
        "./assets/image/monster/Elite/BULLET_UR.png",
        "./assets/image/monster/Elite/BULLET_DL.png",
        "./assets/image/monster/Elite/BULLET_DR.png"
    };

    mode = BossMode::IDLE;
    ImageCenter* IC = ImageCenter::get_instance();
    ALLEGRO_BITMAP* bmp = IC->get(idle_img[0]);
    if (bmp) {
        
        float w = al_get_bitmap_width(bmp);
        float h = al_get_bitmap_height(bmp);

        double r = std::min(w, h) * 0.4f;    

        // 把原本 Monster 設的 shape 替換掉
        shape.reset(new Circle{p.x, p.y, r});
    }
}

void MonsterElite::update() {
  DataCenter* DC = DataCenter::get_instance();
  
    _update_animation(DC);
    _update_motion(DC);
}

void MonsterElite::draw() {
    ImageCenter* IC = ImageCenter::get_instance();
    ALLEGRO_BITMAP* bmp = nullptr;

    if (mode == BossMode::IDLE) {
        bmp = IC->get(idle_img[anim_frame]);
    } else {
        bmp = IC->get(move_img[anim_frame]);
    }

    if (!bmp) return;
    float x = shape->center_x() - al_get_bitmap_width(bmp) / 2;
    float y = shape->center_y() - al_get_bitmap_height(bmp) / 2;
    if (mode == BossMode::ULT_CHARGE) {
    float amp = 4.0f; // 抖動幅度
    x += std::sin(ult_shake_phase * 60.0) * amp;
    y += std::cos(ult_shake_phase * 55.0) * amp;
}
      
    if (hit_flash_timer > 0 || mode == BossMode::ULT_CHARGE) {
      // 紅色偏亮，alpha=1 代表完全不透明
      ALLEGRO_COLOR tint = al_map_rgba_f(1.0, 0.3, 0.3, 1.0);
      al_draw_tinted_bitmap(bmp, tint, x, y, 0);
    } else {
      al_draw_bitmap(bmp, x, y, 0);
    }

}



void MonsterElite::special_ability(DataCenter* DC) {
    double now = al_get_time();
    if(mode == BossMode::ULT_CHARGE || mode == BossMode::ULT_FIRE || mode == BossMode::ULT_MOVE) return;
    // run into hero
    if (mode != BossMode::DASH && now - last_dash_time >= dash_cooldown) {
        _enter_dash_toward_hero(DC);
        last_dash_time = now;
    }

    // shoot diagonal bullets while dashing
    if (mode == BossMode::DASH && dash_timer < dash_duration) {
        if (now - last_shot_time >= shot_cooldown / 3) {
            last_shot_time = now;
            _fire_diagonal_barrage(DC);
        }
    }

    // regular shooting
    if (mode != BossMode::DASH) {
        if (now - last_shot_time >= shot_cooldown) {
            last_shot_time = now;

            Point from{ shape->center_x(), shape->center_y() };
            Point to{ DC->hero->shape->center_x(), DC->hero->shape->center_y() };

            Bullet* b = new Bullet(from, to,
                "./assets/image/monster/Elite/BULLET.png",
                260.0, 5, 2500.0);

            DC->monsterBullets.push_back(b);
        }
    }
}


void MonsterElite::_update_motion(DataCenter* DC) {
    double dt = 1.0 / DC->FPS;

    switch (mode) {

    case BossMode::IDLE: {
        vx = vy = 0.0;
        break;
    }

    case BossMode::MOVE: {
        // 如果你真的想要 MOVE，就在這裡決定 vx/vy
        // 例如：慢慢漂移（你也可以改成自己的 AI）
        // vx = 20; vy = 0;

        shape->update_center_x(shape->center_x() + vx * dt);
        shape->update_center_y(shape->center_y() + vy * dt);
        break;
    }

    case BossMode::DASH: {
        dash_timer += dt;

        shape->update_center_x(shape->center_x() + dash_dir_x * dash_speed * dt);
        shape->update_center_y(shape->center_y() + dash_dir_y * dash_speed * dt);

        if (dash_timer >= dash_duration) {
            dash_timer = 0.0;
            dash_count++;

            if (dash_count >= 2) { // 要 10 次就改 10
                dash_count = 0;
                mode = BossMode::ULT_MOVE;

                // reset ult states
                charge_timer = 0.0;
                ult_timer = 0.0;
                ult_shot_timer = 0.0;
                spin_angle = 0.0;
                ult_shake_phase = 0.0;
            } else {
                mode = BossMode::IDLE;
            }
        }
        break;
    }

    case BossMode::ULT_MOVE: {
        double cx = DC->game_field_length * 0.5 + 100;
        double cy = DC->game_field_length * 0.5 - 100;

        double x = shape->center_x();
        double y = shape->center_y();

        double dx = cx - x;
        double dy = cy - y;
        double dist = std::sqrt(dx * dx + dy * dy);

        if (dist < 5.0) {
            mode = BossMode::ULT_CHARGE;
            charge_timer = 0.0;
            ult_shake_phase = 0.0;
            vx = vy = 0.0;
            break;
        }

        double vxm = dx / (dist + 1e-9) * ult_move_speed;
        double vym = dy / (dist + 1e-9) * ult_move_speed;

        shape->update_center_x(x + vxm * dt);
        shape->update_center_y(y + vym * dt);
        break;
    }

    case BossMode::ULT_CHARGE: {
        charge_timer += dt;
        ult_shake_phase += dt;

        if (charge_timer >= charge_duration) {
            mode = BossMode::ULT_FIRE;
            ult_timer = 0.0;
            ult_shot_timer = 0.0;
            spin_angle = 0.0;
        }
        break;
    }

    case BossMode::ULT_FIRE: {
        ult_timer += dt;
        ult_shot_timer += dt;
        spin_angle += spin_speed * dt;

        if (ult_shot_timer >= ult_shot_interval) {
            ult_shot_timer = 0.0;
            _fire_spinning_barrage(DC);
        }

        if (ult_timer >= ult_duration) {
            mode = BossMode::IDLE;
            ult_timer = 0.0;
            ult_shot_timer = 0.0;
            spin_angle = 0.0;
        }
        break;
    }

    default: {
        mode = BossMode::IDLE;
        break;
    }

    } 
}     


void MonsterElite::_enter_dash_toward_hero(DataCenter* DC) {
    Point boss_pos{ shape->center_x(), shape->center_y() };
    Point hero_pos{ DC->hero->shape->center_x(), DC->hero->shape->center_y() };

    double dx = hero_pos.x - boss_pos.x;
    double dy = hero_pos.y - boss_pos.y;
    double len = std::sqrt(dx*dx + dy*dy);
    if (len == 0) len = 1;

    dash_dir_x = dx / len;
    dash_dir_y = dy / len;

    mode = BossMode::DASH;
    dash_timer = 0.0;
}

void MonsterElite::_update_animation(DataCenter* DC) {
    double dt = 1.0 / DC->FPS;
    anim_timer += dt;
    // --- 擊退位移 ---
    if (std::abs(kb_vx) > 1e-3 || std::abs(kb_vy) > 1e-3) {
        shape->update_center_x(shape->center_x() + kb_vx * dt);
        shape->update_center_y(shape->center_y() + kb_vy * dt);

        // 阻尼衰減（數字越小衰減越快）
        kb_vx *= 0.85;
        kb_vy *= 0.85;

        // 小到一定程度就歸零
        if (std::abs(kb_vx) < 5) kb_vx = 0;
        if (std::abs(kb_vy) < 5) kb_vy = 0;
    }

    // --- 受傷閃紅計時 ---
    if (hit_flash_timer > 0) hit_flash_timer -= dt;
    if (hit_flash_timer < 0) hit_flash_timer = 0;

    double frame_dt;
    int frame_count;

    if (mode == BossMode::IDLE) {
        frame_dt = idle_frame_dt;
        frame_count = static_cast<int>(idle_img.size());
    } else {
        // MOVE + DASH 都用移動動畫
        frame_dt = move_frame_dt;
        frame_count = static_cast<int>(move_img.size());
    }

    if (anim_timer >= frame_dt) {
        anim_timer = 0.0;
        anim_frame = (anim_frame + 1) % frame_count;
    }
}

void MonsterElite::_fire_diagonal_barrage(DataCenter* DC) {
    // 取得當前貼圖尺寸與中心點
    ImageCenter* IC = ImageCenter::get_instance();
    const std::string &path = (mode == BossMode::IDLE)
        ? idle_img[anim_frame]
        : move_img[anim_frame];

    ALLEGRO_BITMAP* bmp = IC->get(path);
    if (!bmp) return;

    
    float halfW = al_get_bitmap_width(bmp)  * 0.5f;
    float halfH = al_get_bitmap_height(bmp) * 0.5f;

    Point c{ shape->center_x(), shape->center_y() };

    // 四個槍口（貼圖四角）
    Point muzzleLU{ c.x - halfW, c.y - halfH };
    Point muzzleRU{ c.x + halfW, c.y - halfH };
    Point muzzleLD{ c.x - halfW, c.y + halfH };
    Point muzzleRD{ c.x + halfW, c.y + halfH };

    // 四個方向單位向量（對應 左上/右上/左下/右下）
    // y 往下是正（Allegro 通常是這樣），所以「上」是 -y
    const double inv = 0.70710678118; // 1/sqrt(2)
    Point dirLU{ -inv, -inv };
    Point dirRU{ +inv, -inv };
    Point dirLD{ -inv, +inv };
    Point dirRD{ +inv, +inv };

    // 子彈參數
    double bullet_speed = 500.0;
    int    dmg          = 6;
    double fly_dist     = 2500.0;

    auto shoot = [&](const Point& from, const Point& dir) {
        // 目標點：沿方向打到很遠（Bullet 用 target 來算 vx/vy）
        Point to{ from.x + dir.x * 10000.0, from.y + dir.y * 10000.0 };

        Bullet* b = new Bullet(
            from,
            to,
            "./assets/image/monster/Elite/BULLET_DIAG.png",
            bullet_speed,
            dmg,
            fly_dist
        );
        DC->monsterBullets.push_back(b);
    };

    shoot(muzzleLU, dirLU);
    shoot(muzzleRU, dirRU);
    shoot(muzzleLD, dirLD);
    shoot(muzzleRD, dirRD);
}

void MonsterElite::_fire_spinning_barrage(DataCenter* DC) {
    ImageCenter* IC = ImageCenter::get_instance();
    const std::string &path = (mode == BossMode::IDLE)
        ? idle_img[anim_frame]
        : move_img[anim_frame];

    ALLEGRO_BITMAP* bmp = IC->get(path);
    if (!bmp) return;

    float halfW = al_get_bitmap_width(bmp) * 0.5f;
    float halfH = al_get_bitmap_height(bmp) * 0.5f;

    Point c{ shape->center_x(), shape->center_y() };
    Point muzzleLU{ c.x - halfW, c.y - halfH };
    Point muzzleRU{ c.x + halfW, c.y - halfH };
    Point muzzleLD{ c.x - halfW, c.y + halfH };
    Point muzzleRD{ c.x + halfW, c.y + halfH };

    // 基礎四方向：45°/135°/225°/315°（對角）
    const double inv = 0.70710678118;
    Point baseLU{ -inv, -inv };
    Point baseRU{ +inv, -inv };
    Point baseLD{ -inv, +inv };
    Point baseRD{ +inv, +inv };

    auto rotate = [&](const Point& d, double a) -> Point {
        double ca = std::cos(a), sa = std::sin(a);
        return Point{ d.x * ca - d.y * sa, d.x * sa + d.y * ca };
    };

    // ✅ 用 spin_angle 做偏轉（連續旋轉）
    Point dLU = rotate(baseLU, spin_angle);
    Point dRU = rotate(baseRU, spin_angle);
    Point dLD = rotate(baseLD, spin_angle);
    Point dRD = rotate(baseRD, spin_angle);

    double bullet_speed = 300.0;
    int dmg = 6;
    double fly_dist = 2500.0;

    auto shoot = [&](const Point& from, const Point& dir) {
        Point to{ from.x + dir.x * 10000.0, from.y + dir.y * 10000.0 };
        Bullet* b = new Bullet(
            from, to,
            "./assets/image/monster/Elite/BULLET_DIAG.png",
            bullet_speed, dmg, fly_dist
        );
        DC->monsterBullets.push_back(b);
    };

    shoot(muzzleLU, dLU);
    shoot(muzzleRU, dRU);
    shoot(muzzleLD, dLD);
    shoot(muzzleRD, dRD);
}


