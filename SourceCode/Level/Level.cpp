#include "Level.h"
#include "../Utils.h"
#include "../data/DataCenter.h"
#include "../monsters/Monster.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <array>
#include <cstdio>


using namespace std;

// fixed settings
namespace LevelSetting {
// 背景圖路徑，依照 level 編號來載入
constexpr char level_map_format[] = "./assets/image/scene/Level%d.jpg";

// 生怪間隔
constexpr int monster_spawn_rate = 90;
}; // namespace LevelSetting

void Level::init() {
  level = -1;
  monster_spawn_counter = 0;
  num_of_monsters.clear();
}

/**
 * @brief 依照關卡編號load
 */
void Level::load_level(int lvl) {
  DataCenter *DC = DataCenter::get_instance();

  level = lvl;
  monster_spawn_counter = 0;

  if (background) {
    al_destroy_bitmap(background);
    background = nullptr;
  }

  // load map
  char img_path[64];
  sprintf(img_path, LevelSetting::level_map_format, lvl);
  background = al_load_bitmap(img_path);
  GAME_ASSERT(background != nullptr, "cannot load level background image.");

  num_of_monsters.clear();
  num_of_monsters.resize(static_cast<size_t>(MonsterType::MONSTERTYPE_MAX), 0);

  switch (lvl) {
  case 0: // level 0

    num_of_monsters[static_cast<size_t>(MonsterType::WOLF)] = 5;
    num_of_monsters[static_cast<size_t>(MonsterType::CAVEMAN)] = 5;
    break;
  case 1: // level 1
    num_of_monsters[static_cast<size_t>(MonsterType::WOLF)] = 10;
    break;
  case 2: // level 2
    num_of_monsters[static_cast<size_t>(MonsterType::SPLIT)] = 20;
    break;
  case 3: // level 3
    num_of_monsters[static_cast<size_t>(MonsterType::SLIME)] = 20;
    num_of_monsters[static_cast<size_t>(MonsterType::SUICIDE)] = 20;
    break;
  default:
    break;
  }

  debug_log("<Level> load level %d.\n", lvl);
}

/**
 * @brief 控制怪物出現的時機，照 num_of_monsters 決定要生哪種怪
 */
void Level::update() {
  if (monster_spawn_counter) {
    monster_spawn_counter--;
    return;
  }

  DataCenter *DC = DataCenter::get_instance();

  for (size_t i = 0; i < num_of_monsters.size(); ++i) {
    if (num_of_monsters[i] == 0)
      continue;

    // 這裡的 create_monster 版本要改成「不吃 road_path」
    // 原本是 Monster::create_monster(type, road_path)
    // 改成 Monster::create_monster(type) 或 Monster::create_monster(type,
    // spawn_pos)
    DC->monsters.emplace_back(
        Monster::create_monster(static_cast<MonsterType>(i), Point{0, 300}));
    num_of_monsters[i]--;
    break;
  }

  monster_spawn_counter = LevelSetting::monster_spawn_rate;
}

/**
 * @brief 畫出整張關卡背景
 */
void Level::draw() {
  DataCenter *DC = DataCenter::get_instance();
  al_draw_scaled_bitmap(background, 0, 0, al_get_bitmap_width(background),
                        al_get_bitmap_height(background), 0, 0,
                        DC->window_width, DC->window_height, 0);
}
