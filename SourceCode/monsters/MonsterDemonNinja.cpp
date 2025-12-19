#include "MonsterDemonNinja.h"
#include "../data/DataCenter.h"
#include "../data/ImageCenter.h"
#include "../Effect.h"
#include "../Utils.h"
#include <allegro5/allegro.h>

// MonsterDemonNinja.cpp
void MonsterDemonNinja::special_ability(DataCenter* DC) {
    if (attack_cd > 0 || is_attacking) return;

    // overlap 判定：用你的 shape overlap
    if (shape->overlap(*DC->hero->shape)) {
        is_attacking = true;
        attack_timer = 0.30;   // 攻擊動作持續 0.3s
        attack_cd = 2.0;       // 2 秒才能再打一次


        // DC->player->HP--;
        // DC->hero->apply_knockback({shape->center_x(), shape->center_y()}, 520, 0.2);
    }
}


void MonsterDemonNinja::update() {
    DataCenter* DC = DataCenter::get_instance();
    double dt = 1.0 / DC->FPS;

    // cooldown
    if (attack_cd > 0) attack_cd -= dt;

    // 攻擊中：只播攻擊動畫，不要跑 Monster::update() 的走路動畫/追蹤
    if (is_attacking) {
        attack_timer -= dt;
        attack_anim_timer += dt;

        if (attack_anim_timer >= 0.10) {  // 每幀 0.1s
            attack_anim_timer = 0.0;
            attack_frame = (attack_frame + 1) % 3; // 0~2
        }

        if (attack_timer <= 0) {
            is_attacking = false;
            attack_frame = 0;
        }
        return;
    }

    // 平常照舊走路
    Monster::update();
}

void MonsterDemonNinja::draw() {
    if (is_attacking) {
        ImageCenter* IC = ImageCenter::get_instance();
        std::string path = "assets/image/monster/DemonNinja/ATTACK_" + std::to_string(attack_frame) + ".png";
        ALLEGRO_BITMAP* bmp = IC->get(path);
        if (!bmp) return;

        al_draw_bitmap(bmp,
            shape->center_x() - al_get_bitmap_width(bmp)/2,
            shape->center_y() - al_get_bitmap_height(bmp)/2,
            0);
        return;
    }
    Monster::draw();
}






