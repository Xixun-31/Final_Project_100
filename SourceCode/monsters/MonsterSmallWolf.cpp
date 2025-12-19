#include "MonsterSmallWolf.h"
#include "../data/DataCenter.h"
#include "../data/ImageCenter.h"
#include "../Effect.h"

void MonsterSmallWolf::special_ability(DataCenter* DC) {
    if (HP <= 0) {
        Point pos{ shape->center_x(), shape->center_y() };
        EffectEvent effect_event2{EffectType::SLIME_DEATH, pos};
        DC->effectEvents.push_back(effect_event2);
       
    }
}




