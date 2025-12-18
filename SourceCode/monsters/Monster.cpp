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
#include "MonsterBarrel.h"


#include "../Hero.h" 
#include "../Utils.h"
#include "../data/DataCenter.h"
#include "../data/ImageCenter.h"
#include "../shapes/Point.h"
#include "../shapes/Rectangle.h"
#include "../Effect.h"
#include "../towers/Bullet.h"


#include <algorithm>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_primitives.h>
#include <cmath>
#include <cstdio>


using namespace std;

// fixed settings
namespace MonsterSetting {
static constexpr char monster_imgs_root_path[static_cast<int>(
    MonsterType::MONSTERTYPE_MAX)][40] = {
    "./assets/image/monster/Wolf","./assets/image/monster/Slime","./assets/image/monster/CaveMan",
    "./assets/image/monster/WolfKnight", "./assets/image/monster/DemonNinja",
    // 之後要實作再打開
    "./assets/image/monster/Bird", "./assets/image/monster/Elite",
    "./assets/image/monster/Slime", "./assets/image/monster/Split",
    "./assets/image/monster/Suicide", "./assets/image/monster/barrel"};

static constexpr char dir_path_prefix[][10] = {"UP", "DOWN", "LEFT", "RIGHT"};
} // namespace MonsterSetting

/**
 * @brief 工廠：依照 type 建對應子類物件（不再吃 path）
 */
Monster *Monster::create_monster(MonsterType type, const Point &p) {
  switch (type) {
  case MonsterType::WOLF:
    return new MonsterWolf{p};
  case MonsterType::SMALLWOLF:
    return new MonsterSmallWolf{p};  
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
  case MonsterType::BARREL:
    return new MonsterBarrel{p};
  case MonsterType::MONSTERTYPE_MAX:
    break;
  }
  GAME_ASSERT(false, "monster type error.");
  return nullptr;
}

MonsterType Monster::peek_type() const {return type;}

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

static int pick_draw_dir(const std::vector<int> bitmap_img_ids[4], int want_dir) {
    if (!bitmap_img_ids[want_dir].empty()) return want_dir;

    // fallback 順序你可調：先用 DOWN，再 RIGHT，再 LEFT，再 UP
    const int order[4] = {(int)Dir::DOWN, (int)Dir::RIGHT, (int)Dir::LEFT, (int)Dir::UP};
    for (int d : order) {
        if (!bitmap_img_ids[d].empty()) return d;
    }
    return want_dir; // 全空才會回到原本（但這代表你根本沒設圖）
}


void Monster::update() {
    DataCenter *DC = DataCenter::get_instance();
    ImageCenter *IC = ImageCenter::get_instance();
    // 1. 更新動畫（先拿出該方向的 frame 列表）

    double dt = 1.0 / DC->FPS;

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



    int want_dir = (int)dir;
    int draw_dir = pick_draw_dir(bitmap_img_ids, want_dir);
    auto &frames = bitmap_img_ids[draw_dir];

  

  if (frames.empty()) {
    debug_log("Monster::update(): no frames for type=%d dir=%d\n",
              (int)type, draw_dir);
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
  int want_dir2 = (int)dir;
  int draw_dir2 = pick_draw_dir(bitmap_img_ids, want_dir2);

std::sprintf(buffer, "%s/%s_%d.png",
    MonsterSetting::monster_imgs_root_path[(int)type],
    MonsterSetting::dir_path_prefix[draw_dir2],
    bitmap_img_ids[draw_dir2][bitmap_img_id]);


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
  int want_dir = (int)dir;
int draw_dir = pick_draw_dir(bitmap_img_ids, want_dir);
auto &frames = bitmap_img_ids[draw_dir];

if (frames.empty()) return; // 全空就不畫

if (bitmap_img_id < 0 || bitmap_img_id >= (int)frames.size())
    bitmap_img_id = 0;

char buffer[50];
std::sprintf(buffer, "%s/%s_%d.png",
    MonsterSetting::monster_imgs_root_path[(int)type],
    MonsterSetting::dir_path_prefix[draw_dir],
    frames[bitmap_img_id]);

    ALLEGRO_BITMAP *bitmap = IC->get(buffer);
    
    float x = shape->center_x() - al_get_bitmap_width(bitmap) / 2;
    float y = shape->center_y() - al_get_bitmap_height(bitmap) / 2;
    al_draw_bitmap(bitmap,x, y, 0);
    
    if (hit_flash_timer > 0) {
      // 紅色偏亮，alpha=1 代表完全不透明
      ALLEGRO_COLOR tint = al_map_rgba_f(1.0, 0.3, 0.3, 1.0);
      al_draw_tinted_bitmap(bitmap, tint, x, y, 0);
    } else {
      al_draw_bitmap(bitmap, x, y, 0);
    }


}



int Monster::get_money() const { return money; }

void Monster::special_ability(DataCenter* DC) {
 
}

void Monster::on_hit(const Point& from, double kb_strength) {
    // 受傷變紅 0.12 秒
    hit_flash_timer = 0.12;

    // 擊退方向：從子彈來源 -> 怪物（把怪往外推）
    double cx = shape->center_x();
    double cy = shape->center_y();
    double dx = cx - from.x;
    double dy = cy - from.y;
    double len = std::sqrt(dx*dx + dy*dy);
    if (len < 1e-6) len = 1.0;

    dx /= len; dy /= len;

    // 給一個瞬間速度（px/s）
    kb_vx += dx * kb_strength;
    kb_vy += dy * kb_strength;
}





