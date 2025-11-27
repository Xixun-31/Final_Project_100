#ifndef MONSTERSPLIT_H_INCLUDED
#define MONSTERSPLIT_H_INCLUDED

#include "Monster.h"

// fixed settings: MonsterSplit attributes
class MonsterSplit : public Monster
{
public:
	MonsterSplit(const std::vector<Point> &path) : Monster{path, MonsterType::SPLIT} {
		HP = 25;
		v = 40;
		money = 20;
		bitmap_img_ids.emplace_back(std::vector<int>({0, 1, 2, 3})); // UP
		bitmap_img_ids.emplace_back(std::vector<int>({0, 1, 2, 3})); // DOWN
		bitmap_img_ids.emplace_back(std::vector<int>({0, 1, 2, 3})); // LEFT
		bitmap_img_ids.emplace_back(std::vector<int>({0, 1, 2, 3})); // RIGHT
		bitmap_switch_freq = 20;
	}
	void split();
};

#endif
