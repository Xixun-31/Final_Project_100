#include "OperationCenter.h"
#include "../Hero.h"
#include "../Player.h"
#include "../Utils.h"
#include "../monsters/Monster.h"
#include "../towers/Bullet.h"
#include "../towers/Tower.h"
#include "../Table.h" // Corrected path
#include "DataCenter.h"
#include "../data/SoundCenter.h"
#include "../Effect.h"
#include <allegro5/allegro_audio.h>
void OperationCenter::update() {

  // Update monsters.
  _update_monster();
  // Update towers.
  _update_tower();
  // Update tower bullets.
  _update_towerBullet();
  // If any bullet overlaps with any monster, we delete the bullet, reduce the
  // HP of the monster, and delete the monster if necessary.
  _update_monster_towerBullet();
  // If any monster reaches the end, hurt the player and delete the monster.
  _update_monster_player();
  // If any hero attacks any monster, we delete the monster.
  _update_hero_monster();
  _update_heroBullet();
  _update_monster_heroBullet();
  _update_monsterBullet();
  _update_hero_monsterBullet();
}

void OperationCenter::_update_monster() {
  std::vector<Monster *> &monsters = DataCenter::get_instance()->monsters;
  for (Monster *monster : monsters) {
    monster->update();
    monster->special_ability(DataCenter::get_instance());
  }
}

void OperationCenter::_update_tower() {
  std::vector<Tower *> &towers = DataCenter::get_instance()->towers;
  for (Tower *tower : towers)
    tower->update();
}

void OperationCenter::_update_towerBullet() {
  std::vector<Bullet *> &towerBullets =
      DataCenter::get_instance()->towerBullets;
  for (Bullet *towerBullet : towerBullets)
    towerBullet->update();
  // Detect if a bullet flies too far (exceeds its fly distance limit), which
  // means the bullet lifecycle has ended.
  for (size_t i = 0; i < towerBullets.size(); ++i) {
    if (towerBullets[i]->get_fly_dist() <= 0) {
      delete towerBullets[i];
      towerBullets.erase(towerBullets.begin() + i);
      --i;
    }
  }
}

void OperationCenter::_update_monster_towerBullet() {
  DataCenter *DC = DataCenter::get_instance();
  std::vector<Monster *> &monsters = DC->monsters;
  std::vector<Bullet *> &towerBullets = DC->towerBullets;
  for (size_t i = 0; i < monsters.size(); ++i) {
    for (size_t j = 0; j < towerBullets.size(); ++j) {
      // Check if the bullet overlaps with the monster.
      if (monsters[i]->shape->overlap(*(towerBullets[j]->shape))) {
        // Reduce the HP of the monster. Delete the bullet.
        monsters[i]->HP -= towerBullets[j]->get_dmg();
        delete towerBullets[j];
        towerBullets.erase(towerBullets.begin() + j);
        --j;
      }
    }
  }
}

void OperationCenter::_update_monster_heroBullet() {
  DataCenter *DC = DataCenter::get_instance();
  std::vector<Monster *> &monsters = DC->monsters;
  std::vector<Bullet *> &heroBullets = DC->heroBullets;
  for (size_t i = 0; i < heroBullets.size(); ++i) {
      bool deleted = false;
      // Check Table Collision
      for(Table *table : DC->tables) {
          if(!table->is_active) continue;
          if(heroBullets[i]->shape->overlap(*(table->shape))) {
              delete heroBullets[i];
              heroBullets.erase(heroBullets.begin() + i);
              deleted = true;
              --i;
              break;
          }
      }
      if(deleted) continue;
      
    for (size_t j = 0; j < monsters.size(); ++j) {
      // Check if the bullet overlaps with the monster.
      if (monsters[j]->shape->overlap(*(heroBullets[i]->shape))) {
        // Reduce the HP of the monster. Delete the bullet.
        monsters[j]->HP -= heroBullets[i]->get_dmg();
        Point bullet_pos{ heroBullets[i]->shape->center_x(), heroBullets[i]->shape->center_y() };
        monsters[j]->on_hit(bullet_pos, 350.0); // 350 = 擊退強度，你可調
        
        delete heroBullets[i];
        heroBullets.erase(heroBullets.begin() + i);
        --i;
        break; // Bullet is gone, stop checking other monsters
      }
    }

  }
}

void OperationCenter::_update_hero_monsterBullet() {
  DataCenter *DC = DataCenter::get_instance();
  Hero *hero = DC->hero;
  Player *player = DC->player;
  std::vector<Bullet*>& bullets = DC->monsterBullets;
  for (size_t i = 0; i < bullets.size(); ++i) {
    bool deleted = false;
    // Check Table Collision
    for(Table *table : DC->tables) {
        if(!table->is_active) continue;
        if(bullets[i]->shape->overlap(*(table->shape))) {
            delete bullets[i];
            bullets.erase(bullets.begin() + i);
            deleted = true;
            --i;
            break;
        }
    }
    if(deleted) continue;

    // Check if the bullet overlaps with the hero.
    if (hero->shape->overlap(*(bullets[i]->shape))) {
      // Reduce the HP of the hero. Delete the bullet.
      Point from{ bullets[i]->shape->center_x(), bullets[i]->shape->center_y() };
      DC->hero->apply_knockback(from, 380.0, 0.12);

      hero->hit();
      delete bullets[i];
      bullets.erase(bullets.begin() + i);
      --i;
    }
  }
}

void OperationCenter::_update_monsterBullet() {
    DataCenter* DC = DataCenter::get_instance();
    auto& bullets = DC->monsterBullets;
    Hero* hero = DC->hero;

    // 先更新子彈位置
    for (Bullet* b : bullets)
        b->update();

    // Shockwave 判定
    bool shock_on = hero->is_shockwave_active();
    float shock_r = hero->getShockwaveRadius();
    float hx = hero->shape->center_x();
    float hy = hero->shape->center_y();

    for (size_t i = 0; i < bullets.size(); ++i) {
        Bullet* b = bullets[i];

        // 1. 被震波擋掉
        if (shock_on && shock_r > 0.0f) {
            float bx = b->shape->center_x();
            float by = b->shape->center_y();
            float dx = bx - hx;
            float dy = by - hy;

            if (dx*dx + dy*dy <= shock_r * shock_r) {
                // 這顆子彈被 shockwave 擋掉
                delete b;
                bullets.erase(bullets.begin() + i);
                --i;
                continue;
            }
        }

        // 2. 正常射程結束
        if (b->get_fly_dist() <= 0) {
            delete b;
            bullets.erase(bullets.begin() + i);
            --i;
        }
    }
}


void OperationCenter::_update_hero_monster() {
  DataCenter *DC = DataCenter::get_instance();
  std::vector<Monster *> &monsters = DC->monsters;
  Hero *hero = DC->hero;
  for (size_t i = 0; i < monsters.size(); ++i) {
    // Check if the hero overlaps with the monster.

    if (hero->shape->overlap(*(monsters[i]->shape)) && monsters[i]->peek_type() != MonsterType::TREASURE) {
      if (monsters[i]->peek_type() == MonsterType::SUICIDE) {
          monsters[i]->HP = 0;
          Effect::emit_suicide_explosion(Point{monsters[i]->shape->center_x(), monsters[i]->shape->center_y()});
      }
      if (monsters[i]->peek_type() == MonsterType::DEMONNINJA) {
          Point from{ monsters[i]->shape->center_x(), monsters[i]->shape->center_y() };
          DC->hero->apply_knockback(from, 520.0, 0.20);  // 力度/時間自己調
      }
      // If hero is not invincible, hurt the player and make hero invincible.
      if (!hero->is_invincible()) {
        hero->hit();
      }
    }
  }
}

void OperationCenter::_update_monster_player() {
  DataCenter *DC = DataCenter::get_instance();
  std::vector<Monster *> &monsters = DC->monsters;
  Player *&player = DC->player;
  for (size_t i = 0; i < monsters.size(); ++i) {
    // Check if the monster is killed.
    if (monsters[i]->HP <= 0) {
      monsters[i]->special_ability(DC);
      // Monster gets killed. Player receives money.
      Point pos(monsters[i]->shape->center_x(), monsters[i]->shape->center_y());
      
      // Play death sound for non-treasure and non-barrel monsters
      if(monsters[i]->peek_type() != MonsterType::TREASURE && monsters[i]->peek_type() != MonsterType::BARREL) {
          SoundCenter::get_instance()->play("./assets/sound/die.WAV", ALLEGRO_PLAYMODE_ONCE);
      }
      if (monsters[i]->peek_type() == MonsterType::CAVEMAN) {
          Effect::emit_SSR_death(pos);
      } else if (monsters[i]->peek_type() == MonsterType::SLIME || monsters[i]->peek_type() == MonsterType::SMALLWOLF) {
          Effect::emit_slime_death(pos);
      } else if (monsters[i]->peek_type() == MonsterType::WOLF || monsters[i]->peek_type() == MonsterType::BARREL) {
          // Wolf/Barrel triggers their own effects in special_ability
      } else {
          Effect::emit_death(pos);
      }

      player->coin += monsters[i]->get_money();
      delete monsters[i];
      monsters.erase(monsters.begin() + i);
      --i;
      // Since the current monsster is killed, we can directly proceed to next
      // monster.
      break;
    }
  }
}

void OperationCenter::draw() {
  _draw_monster();
  _draw_tower();
  _draw_towerBullet();
  _draw_heroBullet();
  _draw_effect();
  _draw_monsterBullet();
}

void OperationCenter::_draw_monster() {
  std::vector<Monster *> &monsters = DataCenter::get_instance()->monsters;
  for (Monster *monster : monsters)
    monster->draw();
}

void OperationCenter::_draw_tower() {
  std::vector<Tower *> &towers = DataCenter::get_instance()->towers;
  for (Tower *tower : towers)
    tower->draw();
}

void OperationCenter::_draw_towerBullet() {
  std::vector<Bullet *> &towerBullets =
      DataCenter::get_instance()->towerBullets;
  for (Bullet *towerBullet : towerBullets)
    towerBullet->draw();
}

void OperationCenter::_update_heroBullet() {
  std::vector<Bullet *> &heroBullets = DataCenter::get_instance()->heroBullets;
  for (Bullet *heroBullet : heroBullets)
    heroBullet->update();

  // Remove dead bullets
  for (size_t i = 0; i < heroBullets.size(); ++i) {
    if (heroBullets[i]->get_fly_dist() <= 0) {
      delete heroBullets[i];
      heroBullets.erase(heroBullets.begin() + i);
      --i;
    }
  }
}

void OperationCenter::_draw_heroBullet() {
  std::vector<Bullet *> &heroBullets = DataCenter::get_instance()->heroBullets;
  for (Bullet *heroBullet : heroBullets)
    heroBullet->draw();
}

void OperationCenter::_draw_monsterBullet() {
  std::vector<Bullet*>& bullets = DataCenter::get_instance()->monsterBullets;
  
  for (Bullet* b : bullets)
    b->draw();
}

void OperationCenter::_draw_effect() {
  Effect::draw_all();
}