#include "Monster.h"
#include "MonsterBird.h"
#include "MonsterCaveMan.h"
#include "MonsterDemonNinja.h"
#include "MonsterElite.h"
#include "MonsterSlime.h"
#include "MonsterSplit.h"
#include "MonsterSuicide.h"
#include "MonsterWolf.h"
#include "MonsterWolfKnight.h"
#include "MonsterSmallWolf.h"


#include "../Hero.h" 
#include "../Utils.h"
#include "../data/DataCenter.h"
#include "../data/ImageCenter.h"
#include "../shapes/Point.h"
#include "../shapes/Rectangle.h"
#include "../Effect.h"
#include "../towers/Bullet.h"


#include <algorithm>
#include <allegro5/allegro_primitives.h>
#include <cmath>
#include <cstdio>


using namespace std;

// fixed settings
namespace MonsterSetting {
static constexpr char monster_imgs_root_path[static_cast<int>(
    MonsterType::MONSTERTYPE_MAX)][40] = {
    "./assets/image/monster/Wolf","./assets/image/monster/Wolf","./assets/image/monster/CaveMan",
    "./assets/image/monster/WolfKnight", "./assets/image/monster/DemonNinja",
    // 之後要實作再打開
    "./assets/image/monster/Bird", "./assets/image/monster/Elite",
    "./assets/image/monster/Slime", "./assets/image/monster/Split",
    "./assets/image/monster/Suicide"};

static constexpr char dir_path_prefix[][10] = {"UP", "DOWN", "LEFT", "RIGHT"};
} // namespace MonsterSetting

/**
 * @brief 工廠：依照 type 建對應子類物件（不再吃 path）
 */
Monster *Monster::create_monster(MonsterType type, const Point &p) {
  switch (type) {
  case MonsterType::WOLF:
    return new MonsterWolf{p};
  case MonsterType::CAVEMAN:
    return new MonsterCaveMan{p};
  case MonsterType::WOLFKNIGHT:
    return new MonsterWolfKnight{p};
  case MonsterType::DEMONNINJA:
    return new MonsterDemonNinja{p};
  case MonsterType::BIRD:
    return new MonsterBird{p};
  case MonsterType::ELITE:
    return new MonsterElite{p};
  case MonsterType::SLIME:
    return new MonsterSlime{p};
  case MonsterType::SPLIT:
    return new MonsterSplit{p};
  case MonsterType::SUICIDE:
    return new MonsterSuicide{p};
  case MonsterType::MONSTERTYPE_MAX:
    break;
  }
  GAME_ASSERT(false, "monster type error.");
  return nullptr;
}

/**
 * @brief 將一個位移向量 v 換成面向方向
 */
Dir convert_dir(const Point &v) {
  if (v.y < 0 && std::abs(v.y) >= std::abs(v.x))
    return Dir::UP;
  if (v.y > 0 && std::abs(v.y) >= std::abs(v.x))
    return Dir::DOWN;
  if (v.x < 0 && std::abs(v.x) >= std::abs(v.y))
    return Dir::LEFT;
  if (v.x > 0 && std::abs(v.x) >= std::abs(v.y))
    return Dir::RIGHT;
  return Dir::RIGHT;
}

/**
 * @brief 基底建構子：不再吃 path，只把 type
 * 記下來，設定初始位置在畫面左邊中間（你可以自己改）
 */
Monster::Monster(MonsterType type, const Point &p) {
  

  this->type = type;
  dir = Dir::RIGHT;
  bitmap_img_id = 0;
  bitmap_switch_counter = 0;
  bitmap_switch_freq = 10; // 子類別可以 override

  // 初始位置：畫面左邊中線，你可以改成隨機 y 之類
  double spawn_x = p.x;
  double spawn_y = p.y;
  shape.reset(new Rectangle{spawn_x, spawn_y, spawn_x, spawn_y});
}

/**
 * @details 更新：動畫 → 追玩家移動 → 更新 hitbox
 */
void Monster::update() {
    DataCenter *DC = DataCenter::get_instance();
    ImageCenter *IC = ImageCenter::get_instance();
    // 1. 更新動畫（先拿出該方向的 frame 列表）
    int d = static_cast<int>(dir);
  auto &frames = bitmap_img_ids[d];

  if (frames.empty()) {
    debug_log("Monster::update(): no frames for type=%d dir=%d\n",
              (int)type, d);
    // 沒有對這個方向設定任何 frame，先不要更新動畫，避免崩潰
    bitmap_img_id = 0;
  } else {
    // 保證 bitmap_img_id 在合法範圍內
    if (bitmap_img_id < 0 || bitmap_img_id >= (int)frames.size()) {
      debug_log("Monster::update(): bitmap_img_id=%d out of range, reset to 0\n",
                bitmap_img_id);
      bitmap_img_id = 0;
    }

    if (bitmap_switch_counter)
      --bitmap_switch_counter;
    else {
      bitmap_img_id = (bitmap_img_id + 1) % frames.size();
      bitmap_switch_counter = bitmap_switch_freq;
    }
  }
  // 2. 計算這一幀可以移動的距離
  double movement = v / DC->FPS;

  // 3. 自動朝 hero 靠近
  double hero_x = DC->hero->shape->center_x();
  double hero_y = DC->hero->shape->center_y();

  const double attack_range = 5.0; // 距離小於這個就當作「到達」不再硬貼上去

  while (movement > 0) {
    double cx = shape->center_x();
    double cy = shape->center_y();

    double dx = hero_x - cx;
    double dy = hero_y - cy;
    double d = std::sqrt(dx * dx + dy * dy);

    // 已經幾乎重合
    if (d < 1e-3)
      break;

    // 進入攻擊距離：只轉向，不再前進
    if (d <= attack_range) {
      dir = convert_dir(Point{dx, dy});
      break;
    }

    // 實際移動距離：不超過自身 movement，也不要超過 (d - attack_range)
    double move_dist = std::min(movement, d - attack_range);

    double move_dx = dx / d * move_dist;
    double move_dy = dy / d * move_dist;

    // 更新朝向
    Dir tmpdir = convert_dir(Point{move_dx, move_dy});
    dir = tmpdir;

    // 更新位置
    shape->update_center_x(cx + move_dx);
    shape->update_center_y(cy + move_dy);

    movement -= move_dist;

    break;
  }

  // 4. 更新 hitbox
  char buffer[50];
  std::sprintf(buffer, "%s/%s_%d.png",
               MonsterSetting::monster_imgs_root_path[static_cast<int>(type)],
               MonsterSetting::dir_path_prefix[static_cast<int>(dir)],
               bitmap_img_ids[static_cast<int>(dir)][bitmap_img_id]);
  ALLEGRO_BITMAP *bitmap = IC->get(buffer);

  const double cx = shape->center_x();
  const double cy = shape->center_y();

  int w = al_get_bitmap_width(bitmap) * 0.8;
  int h = al_get_bitmap_height(bitmap) * 0.8;

  shape.reset(new Rectangle{(cx - w / 2.0), (cy - h / 2.0), (cx - w / 2.0 + w),
                            (cy - h / 2.0 + h)});
}

void Monster::draw() {
 ImageCenter *IC = ImageCenter::get_instance();
  int d = static_cast<int>(dir);
  auto &frames = bitmap_img_ids[d];

  if (frames.empty()) {
    debug_log("Monster::draw(): no frames for type=%d dir=%d\n",
              (int)type, d);
    return;
  }
  if (bitmap_img_id < 0 || bitmap_img_id >= (int)frames.size()) {
    debug_log("Monster::draw(): bitmap_img_id=%d out of range, reset to 0\n",
              bitmap_img_id);
    bitmap_img_id = 0;
  }

  char buffer[50];
  std::sprintf(buffer, "%s/%s_%d.png",
               MonsterSetting::monster_imgs_root_path[static_cast<int>(type)],
               MonsterSetting::dir_path_prefix[d],
               frames[bitmap_img_id]);

  debug_log("Drawing monster of type %d, dir %d, frame index %d (file id %d)\n",
            (int)type, d, bitmap_img_id, frames[bitmap_img_id]);
  ALLEGRO_BITMAP *bitmap = IC->get(buffer);
  al_draw_bitmap(bitmap, shape->center_x() - al_get_bitmap_width(bitmap) / 2,
                 shape->center_y() - al_get_bitmap_height(bitmap) / 2, 0);
}

int Monster::get_money() const { return money; }

void Monster::special_ability(DataCenter* DC) {
 
}
void MonsterWolf::special_ability(DataCenter* DC) {
    if (HP <= 0 && !splited) {
        splited = true;

        Point pos1{shape->center_x() + 50, shape->center_y()};
        Point pos2{shape->center_x() - 50, shape->center_y()};

        Monster *s1 = new MonsterSmallWolf(pos1);
        Monster *s2 = new MonsterSmallWolf(pos2);
        DC->monsters.push_back(s1);
        DC->monsters.push_back(s2);

        // 丟一個 SPLIT 特效事件（Effect 系統會負責畫）
        Effect::emit_split(Point{shape->center_x(), shape->center_y()});
    }
}
void MonsterCaveMan::special_ability(DataCenter* DC) {
    if (HP <= 0) return;  // 死了就不要射
  debug_log("CaveMan special ability called.\n");
    double now = al_get_time();

    // 1. 處理「開槍動作」播放時間（例如 0.3 秒）
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
    double range = 500.0;     // 射程

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
