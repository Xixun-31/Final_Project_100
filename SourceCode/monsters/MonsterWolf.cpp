#include "MonsterWolf.h"
#include "../data/DataCenter.h"
#include "../data/ImageCenter.h"
#include "../Effect.h"

void MonsterWolf::special_ability(DataCenter* DC) {
    if (HP <= 0 && !splited) {
        splited = true;
        // 分裂成兩隻小狼
        Point pos1{ shape->center_x() + 20, shape->center_y() };
        Point pos2{ shape->center_x() - 20, shape->center_y() };
        Point pos{ shape->center_x(), shape->center_y() };
        Monster* small_wolf1 = create_monster(MonsterType::SMALLWOLF, pos1);
        Monster* small_wolf2 = create_monster(MonsterType::SMALLWOLF, pos2);
        
        Effect::emit_split(pos); // Use helper function
        
        DC->monsters.push_back(small_wolf1);
        DC->monsters.push_back(small_wolf2);
    }
}