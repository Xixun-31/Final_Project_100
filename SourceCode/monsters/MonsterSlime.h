#ifndef MONSTERSLIME_H_INCLUDED
#define MONSTERSLIME_H_INCLUDED

#include "Monster.h"

// fixed settings: MonsterSlime attributes
class MonsterSlime : public Monster
{
public:
	MonsterSlime(const std::vector<Point> &path) : Monster{path, MonsterType::SLIME} {
		HP = 25;
		v = 40;
		money = 20;
		bitmap_img_ids.emplace_back(std::vector<int>({0, 1, 2, 3})); // UP
		bitmap_img_ids.emplace_back(std::vector<int>({0, 1, 2, 3})); // DOWN
		bitmap_img_ids.emplace_back(std::vector<int>({0, 1, 2, 3})); // LEFT
		bitmap_img_ids.emplace_back(std::vector<int>({0, 1, 2, 3})); // RIGHT
		bitmap_switch_freq = 20;
	}
	
};

#endif
