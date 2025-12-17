#include "LevelConfig.h"
#include "../monsters/Monster.h" // for MonsterType

LevelConfig make_level0() {
    LevelConfig cfg;
    cfg.id = 0;
    cfg.background_path = "./assets/image/scene/Level0.jpg";
    cfg.spawn_rate = 60;
    cfg.monsters.assign((size_t)MonsterType::MONSTERTYPE_MAX, 0);
    cfg.monsters[(size_t)MonsterType::WOLF] = 2;
    return cfg;
}

