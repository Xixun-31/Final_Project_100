#include "Monster.h"
#include "MonsterWolf.h"
#include "MonsterCaveMan.h"
#include "MonsterWolfKnight.h"
#include "MonsterDemonNinja.h"
#include "MonsterBird.h"
#include "MonsterElite.h"
#include "MonsterSlime.h"
#include "MonsterSplit.h"
#include "MonsterSuicide.h"
#include "../data/DataCenter.h"
#include "../data/ImageCenter.h"
#include "../Level.h"
#include "../shapes/Point.h"
#include "../shapes/Rectangle.h"
#include "../Utils.h"
#include <allegro5/allegro_primitives.h>

using namespace std;

// fixed settings
enum class Dir {
	UP, DOWN, LEFT, RIGHT
};
namespace MonsterSetting {
	static constexpr char monster_imgs_root_path[static_cast<int>(MonsterType::MONSTERTYPE_MAX)][40] = {
		"./assets/image/monster/Wolf",
		"./assets/image/monster/CaveMan",
		"./assets/image/monster/WolfKnight",
		"./assets/image/monster/DemonNinja",
		// skip non-implemented monsters for now 還沒加進去
		"./assets/image/monster/Bird",
		"./assets/image/monster/Elite",
		"./assets/image/monster/Slime",
		"./assets/image/monster/Split",
		"./assets/image/monster/Suicide"
	};
	static constexpr char dir_path_prefix[][10] = {
		"UP", "DOWN", "LEFT", "RIGHT"
	};
}

/**
 * @brief Create a Monster* instance by the type.
 * @param type the type of a monster.
 * @param path walk path of the monster. The path should be represented in road grid format.
 * @return The curresponding Monster* instance.
 * @see Level::grid_to_region(const Point &grid) const
 */
Monster *Monster::create_monster(MonsterType type, const vector<Point> &path) {
	switch(type) {
		case MonsterType::WOLF: {
			return new MonsterWolf{path};
		}
		case MonsterType::CAVEMAN: {
			return new MonsterCaveMan{path};
		}
		case MonsterType::WOLFKNIGHT: {
			return new MonsterWolfKnight{path};
		}
		case MonsterType::DEMONNIJIA: {
			return new MonsterDemonNinja{path};
		}
		case MonsterType::BIRD: {
			return new MonsterBird{path};
		}
		case MonsterType::ELITE: {
			return new MonsterElite{path};
		}
		case MonsterType::SLIME: {
			return new MonsterSlime{path};
		}
		case MonsterType::SPLIT: {
			return new MonsterSplit{path};
		}
		case MonsterType::SUICIDE: {
			return new MonsterSuicide{path};
		}
		case MonsterType::MONSTERTYPE_MAX: {}
	}
	GAME_ASSERT(false, "monster type error.");
}

/**
 * @brief Given velocity of x and y direction, determine which direction the monster should face.
 */
Dir convert_dir(const Point &v) {
	if(v.y < 0 && abs(v.y) >= abs(v.x))
		return Dir::UP;
	if(v.y > 0 && abs(v.y) >= abs(v.x))
		return Dir::DOWN;
	if(v.x < 0 && abs(v.x) >= abs(v.y))
		return Dir::LEFT;
	if(v.x > 0 && abs(v.x) >= abs(v.y))
		return Dir::RIGHT;
	return Dir::RIGHT;
}

Monster::Monster(const vector<Point> &path, MonsterType type) {
	DataCenter *DC = DataCenter::get_instance();

	shape.reset(new Rectangle{0, 0, 0, 0});
	this->type = type;
	dir = Dir::RIGHT;
	bitmap_img_id = 0;
	bitmap_switch_counter = 0;
	for(const Point &p : path)
		this->path.push(p);
	if(!path.empty()) {
		const Point &grid = this->path.front();
		const Rectangle &region = DC->level->grid_to_region(grid);
		// Temporarily set the bounding box to the center (no area) since we haven't got the hit box of the monster.
		shape.reset(new Rectangle{region.center_x(), region.center_y(), region.center_x(), region.center_y()});
		this->path.pop();
	}
}

/**
 * @details This update function updates the following things in order:
 * @details * Move pose of the current facing direction (bitmap_img_id).
 * @details * Current position (center of the hit box). The position is moved based on the center of the hit box (Rectangle). If the center of this monster reaches the center of the first point of path, the function will proceed to the next point of path.
 * @details * Update the real bounding box by the center of the hit box calculated as above.
 */
void Monster::update() {
    DataCenter *DC = DataCenter::get_instance();
    ImageCenter *IC = ImageCenter::get_instance();

    // 1. 更新動畫 
    if (bitmap_switch_counter) --bitmap_switch_counter;
    else {
        bitmap_img_id = (bitmap_img_id + 1) % (bitmap_img_ids[static_cast<int>(dir)].size());
        bitmap_switch_counter = bitmap_switch_freq;
    }

    // 2. 計算這一幀可以移動的距離
    double movement = v / DC->FPS;

    // 3. 自動朝英雄靠近
    double hero_x = DC->hero->shape->center_x();
    double hero_y = DC->hero->shape->center_y();

    const double attack_range = 5.0;  // 你可以調整這個數字

    while (movement > 0) {
        double cx = shape->center_x();
        double cy = shape->center_y();

        double dx = hero_x - cx;
        double dy = hero_y - cy;
        double d  = std::sqrt(dx * dx + dy * dy);

        // 已經在英雄身上了，或距離太小就不動
        if (d < 1e-3) break;

        // 如果已經進入攻擊距離，就只更新方向，不再往前走
        if (d <= attack_range) {
            dir = convert_dir(Point{dx, dy});
            break;
        }

        // 此幀實際移動距離：不超過 movement，也不超過 (d - attack_range)
        double move_dist = std::min(movement, d - attack_range);

        double move_dx = dx / d * move_dist;
        double move_dy = dy / d * move_dist;

        // 更新朝向
        Dir tmpdir = convert_dir(Point{move_dx, move_dy});
        dir = tmpdir;

        // 更新位置
        shape->update_center_x(cx + move_dx);
        shape->update_center_y(cy + move_dy);

        // 扣掉剩餘可移動距離（通常 while 只會跑一次，但這樣寫結構比較通用）
        movement -= move_dist;

        // 這種追玩家通常一幀跑一次就好，直接 break 也可以：
        // break;
    }

    // 4. 更新 hitbox（跟原本一樣）
    char buffer[50];
    std::sprintf(
        buffer, "%s/%s_%d.png",
        MonsterSetting::monster_imgs_root_path[static_cast<int>(type)],
        MonsterSetting::dir_path_prefix[static_cast<int>(dir)],
        bitmap_img_ids[static_cast<int>(dir)][bitmap_img_id]
    );
    ALLEGRO_BITMAP *bitmap = IC->get(buffer);

    const double cx = shape->center_x();
    const double cy = shape->center_y();

    // 讓 hitbox 比圖小一點
    int w = al_get_bitmap_width(bitmap) * 0.8;
    int h = al_get_bitmap_height(bitmap) * 0.8;

    shape.reset(new Rectangle{
        (cx - w / 2.0), (cy - h / 2.0),
        (cx - w / 2.0 + w), (cy - h / 2.0 + h)
    });
}


void Monster::draw() {
	ImageCenter *IC = ImageCenter::get_instance();
	char buffer[50];
	sprintf(
		buffer, "%s/%s_%d.png",
		MonsterSetting::monster_imgs_root_path[static_cast<int>(type)],
		MonsterSetting::dir_path_prefix[static_cast<int>(dir)],
		bitmap_img_ids[static_cast<int>(dir)][bitmap_img_id]);
	ALLEGRO_BITMAP *bitmap = IC->get(buffer);
	al_draw_bitmap(
		bitmap,
		shape->center_x() - al_get_bitmap_width(bitmap) / 2,
		shape->center_y() - al_get_bitmap_height(bitmap) / 2, 0);
}
