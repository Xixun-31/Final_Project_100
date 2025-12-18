#include "MonsterSuicide.h"
#include "../data/DataCenter.h"
#include "../data/ImageCenter.h"
#include "../Effect.h"

void MonsterSuicide::special_ability(DataCenter *DC) {
    if (HP <= 0) {
        Point pos{ shape->center_x(), shape->center_y() };
        Effect::emit_suicide_explosion(pos);
    }
}