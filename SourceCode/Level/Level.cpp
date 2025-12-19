#include "Level.h"
#include "LevelConfig.h"
#include "../Player.h"
#include "../Utils.h"
#include "../data/DataCenter.h"
#include "../monsters/Monster.h"
#include "../towers/Bullet.h"
#include "../towers/Tower.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <array>
#include <cstdio>
#include "../monsters/Monster.h"

using namespace std;

// fixed settings
namespace LevelSetting {
// 背景圖路徑，依照 level 編號來載入
constexpr char level_map_format[] = "./assets/image/scene/Level%d.jpg";
// 生怪間隔
constexpr int monster_spawn_rate = 60;
}; // namespace LevelSetting



void Level::init() {
  DataCenter *DC = DataCenter::get_instance();
  DC->level_counter = 1;
  DC->curr_level = -1;
  DC->player->reset();
  level = -1;
  monster_spawn_counter = LevelSetting::monster_spawn_rate;
  num_of_monsters.clear();
  num_of_monsters.resize(static_cast<size_t>(MonsterType::MONSTERTYPE_MAX), 0);
  background = nullptr;

  for (Monster *monster : DC->monsters) {
    delete monster;
  }
  DC->monsters.clear();

  for (Bullet *bullet : DC->heroBullets) {
    delete bullet;
  }
  DC->heroBullets.clear();
}

/**
 * @brief 依照關卡編號load
 */
void Level::load_level(int lvl) {
  DataCenter *DC = DataCenter::get_instance();

  level = lvl;
  monster_spawn_counter = 0;
  spawn_points.emplace_back(SpawnPoint{Point{0, 0}});
  spawn_points.emplace_back(SpawnPoint{Point{0, DC->game_field_length}});
  spawn_points.emplace_back(SpawnPoint{Point{DC->game_field_length, 0}});
  spawn_points.emplace_back(SpawnPoint{Point{DC->game_field_length, DC->game_field_length}});
  spawn_points.emplace_back(SpawnPoint{Point{DC->game_field_length / 2 + 50, DC->game_field_length / 2}}); // Index 4: Center
  spawn_points.emplace_back(SpawnPoint{Point{DC->game_field_length / 2 + 100, 0}}); // Index 5: Center Left
  
  if (lvl == 0) {
      // Clear any existing monsters (e.g. Barrels from Level 1)
      for(auto m : DC->monsters) {
          delete m;
      }
      DC->monsters.clear();
  }
  
  if (background) {
    al_destroy_bitmap(background);
    background = nullptr;
  }

  LevelConfig cfg = make_level_config(lvl);

  background = al_load_bitmap(cfg.background_path.c_str());
  GAME_ASSERT(background != nullptr, "cannot load level background image.");

  waves.clear();
  wave_idx = 0;
  unit_idx = 0;
  unit_left = 0;

  if (lvl == 0) {
      waves.push_back(Wave{
          .units = {{MonsterType::TREASURE, 1, 1, 4}}, // 1 Treasure at Center (4)
          .spawn_interval = 0,
          .start_delay = 30, // Short delay before appearance
          .wait_until_clear = true
      });
  }
  if (lvl == 1) {
      waves.push_back(Wave{
          .units = {{MonsterType::BARREL, 1, 1, 4}}, // Wave 0: Barrel (Center)
          .spawn_interval = 0,
          .start_delay = 0,
          .wait_until_clear = false // Don't wait for barrel
      });

      waves.push_back(Wave{
          .units = {{MonsterType::SLIME, 1, 1, 0},
                    {MonsterType::SUICIDE, 2, 1, 1},
                    {MonsterType::SUICIDE, 2, 1, 2}, 
                    {MonsterType::DEMONNINJA, 1, 1, 3}
          },
          .spawn_interval = 40,
          .start_delay = 0, // Starts immediately after Barrel wave (which is instant)
          .wait_until_clear = true
      });

      waves.push_back(Wave{
          .units = {{MonsterType::SUICIDE, 3}, {MonsterType::DEMONNINJA, 2}},
          .spawn_interval = 30,
          .start_delay = 120,  // 第二波前等2秒（假設60FPS）
          .wait_until_clear = true
      });
  }
  if (lvl == 2) {
      waves.push_back(Wave{
      .units = {{MonsterType::CAVEMAN, 4}},
      .spawn_interval = 50,
      .start_delay = 60,
      .wait_until_clear = true
      });

      waves.push_back(Wave{
      .units = {{MonsterType::CAVEMAN, 2}, {MonsterType::SUICIDE, 2}},
      .spawn_interval = 40,
      .start_delay = 120,
      .wait_until_clear = true
      });
  }
  if (lvl == 3) {
      waves.push_back(Wave{
      .units = {{MonsterType::ELITE, 1, 1, 5}},
      .spawn_interval = 1,
      .start_delay = 60,
      .wait_until_clear = true
      });
  }


  // 如果你想每關 spawn_rate 不同：把 LevelSetting::monster_spawn_rate 改成成員變數 spawn_rate
  // 目前你就先固定 60 也可以。

  debug_log("<Level> load level %d.\n", lvl);
}


/**
 * @brief 控制怪物出現的時機，照 num_of_monsters 決定要生哪種怪
 */
void Level::update() {
    DataCenter* DC = DataCenter::get_instance();

    if (wave_idx >= (int)waves.size()) return; // 全部波都生完了

    Wave& w = waves[wave_idx];

    // 1) wave 開始延遲
    if (start_delay_counter < w.start_delay) {
        start_delay_counter++;
        return;
    }

    // 2) 如果這波要求「等清場」：且這波生完了，場上還有怪，就等
    //    判斷「這波生完」：unit_idx >= w.units.size()
    bool wave_spawn_finished = (unit_idx >= w.units.size());
    if (w.wait_until_clear && wave_spawn_finished) {
        // if (!DC->monsters.empty()) return; // Old check
        // Ignore Barrel for wait_until_clear
        bool has_blocking_monsters = false;
        for(auto* m : DC->monsters) {
            if(m->peek_type() != MonsterType::BARREL) {
                has_blocking_monsters = true;
                break;
            }
        }
        if (has_blocking_monsters) return;
        // 清完了 -> 進下一波
        wave_idx++;
        unit_idx = 0;
        unit_left = 0;
        start_delay_counter = 0;
        monster_spawn_counter = 0;
        return;
    }

    // 3) 生怪間隔
    if (monster_spawn_counter > 0) {
        monster_spawn_counter--;
        return;
    }

    // 4) 如果目前這個 unit 沒初始化，初始化 unit_left
    if (unit_left == 0) {
        if (unit_idx >= w.units.size()) {
            // 這波生完，但 wait_until_clear=false，那就直接下一波
            wave_idx++;
            unit_idx = 0;
            unit_left = 0;
            start_delay_counter = 0;
            monster_spawn_counter = 0;
            return;
        }
        unit_left = w.units[unit_idx].count;
    }

    // 5) 生怪（一次 burst 隻）
    int k = std::min(w.units[unit_idx].burst, unit_left);

    for (int i = 0; i < k; ++i) {
        Point sp;

        int pid = w.units[unit_idx].spawnPointId;

        if (pid >= 0 && pid < (int)spawn_points.size()) {
            sp = spawn_points[pid].pos;                // 固定點
        } else {
            // 輪流點（推薦，比 random 更好 debug）
            static int rr = 0;
            sp = spawn_points[rr % spawn_points.size()].pos;
            rr++;
        }

        DC->monsters.emplace_back(Monster::create_monster(w.units[unit_idx].type, sp));
    }

    unit_left -= k;
    if (unit_left == 0) unit_idx++;

    monster_spawn_counter = w.spawn_interval;

}


/**
 * @brief 畫出整張關卡背景
 */
void Level::draw() {
  DataCenter *DC = DataCenter::get_instance();
  if (!background) {
    debug_log("Level::draw(): background is null, level=%d\n", level);
    return;
  }
  al_draw_scaled_bitmap(background, 0, 0, al_get_bitmap_width(background),
                        al_get_bitmap_height(background), 0, 0,
                        DC->window_width, DC->window_height, 0);
}

bool Level::all_waves_done() const { return wave_idx >= (int)waves.size(); }

