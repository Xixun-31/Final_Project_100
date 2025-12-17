#include "LevelConfig.h"
#include "../monsters/Monster.h"

LevelConfig make_level2() {
    LevelConfig cfg;
    cfg.id = 2;
    cfg.background_path = "./assets/image/scene/Level2.jpg";
    cfg.spawn_rate = 60;
    cfg.monsters.assign((size_t)MonsterType::MONSTERTYPE_MAX, 0);
    cfg.monsters[(size_t)MonsterType::CAVEMAN] = 2;
    return cfg;
}
