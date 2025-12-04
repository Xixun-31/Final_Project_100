#include "OperationCenter.h"
#include "../Hero.h"
#include "../Player.h"
#include "../Utils.h"
#include "../monsters/Monster.h"
#include "../towers/Bullet.h"
#include "../towers/Tower.h"
#include "DataCenter.h"
#include "../Effect.h"

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
<<<<<<< HEAD

=======
  _update_monster_heroBullet();
>>>>>>> 7f00703e6b5c2ca05a7f6320518e96e0fe6e4e91
}

void OperationCenter::_update_monster() {
  std::vector<Monster *> &monsters = DataCenter::get_instance()->monsters;
  for (Monster *monster : monsters)
    monster->update();
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
  for (size_t i = 0; i < monsters.size(); ++i) {
    for (size_t j = 0; j < heroBullets.size(); ++j) {
      // Check if the bullet overlaps with the monster.
      if (monsters[i]->shape->overlap(*(heroBullets[j]->shape))) {
        // Reduce the HP of the monster. Delete the bullet.
        monsters[i]->HP -= heroBullets[j]->get_dmg();
        delete heroBullets[j];
        heroBullets.erase(heroBullets.begin() + j);
        --j;
      }
    }
  }
}

void OperationCenter::_update_hero_monster() {
  DataCenter *DC = DataCenter::get_instance();
  std::vector<Monster *> &monsters = DC->monsters;
  Hero *hero = DC->hero;
  for (size_t i = 0; i < monsters.size(); ++i) {
    // Check if the hero overlaps with the monster.
    if (hero->shape->overlap(*(monsters[i]->shape))) {
      // If hero is not invincible, hurt the player and make hero invincible.
      if (!hero->is_invincible()) {
        DC->player->HP--;
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
      // Monster gets killed. Player receives money.
      monsters[i]->special_ability(DC);
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

void OperationCenter::_draw_effect() {
    Effect::draw_all();
}