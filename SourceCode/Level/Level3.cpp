#include "LevelConfig.h"
#include "../monsters/Monster.h"

LevelConfig make_level3() {
    LevelConfig cfg;
    cfg.id = 3;
    cfg.background_path = "./assets/image/scene/Level3.jpg";
    cfg.spawn_rate = 60;
    cfg.monsters.assign((size_t)MonsterType::MONSTERTYPE_MAX, 0);
    cfg.monsters[(size_t)MonsterType::ELITE] = 1;
    return cfg;
}
