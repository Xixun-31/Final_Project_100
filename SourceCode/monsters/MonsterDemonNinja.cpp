#include "MonsterDemonNinja.h"
#include "../data/DataCenter.h"
#include "../data/ImageCenter.h"
#include "../Effect.h"

void MonsterDemonNinja::special_ability(DataCenter* DC) {
    bool overlap = shape->overlap(*DC->hero->shape);
    double now = al_get_time();

    if (overlap && !was_overlapping) {
        // 只在剛接觸那一幀觸發
        Point pos{shape->center_x(), shape->center_y()};
        Effect::emit_slap(pos);
        next_slap_time = now + 2.0; // 可留可不留
    }

    was_overlapping = overlap;
}

