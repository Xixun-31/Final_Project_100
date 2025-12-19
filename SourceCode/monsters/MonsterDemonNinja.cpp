#include "MonsterDemonNinja.h"
#include "../data/DataCenter.h"
#include "../data/ImageCenter.h"
#include "../Effect.h"
#include "../Utils.h"

// MonsterDemonNinja.cpp
void MonsterDemonNinja::special_ability(DataCenter* DC) {
    double dt = 1.0 / DC->FPS;
    if (slap_cd > 0) slap_cd -= dt;

    bool now_overlap = shape->overlap(*(DC->hero->shape));

    // 只在「剛碰到」(false->true) 觸發一次
    if (now_overlap && !was_overlapping && slap_cd <= 0) {
        Point pos{ shape->center_x(), shape->center_y() };
        DC->effectEvents.push_back(EffectEvent{EffectType::SLAP, pos, al_get_time(), 0.3});
        debug_log("emit SLAP, events=%zu\n", DC->effectEvents.size());

        slap_cd = 2.0;
    }

    was_overlapping = now_overlap;
}




