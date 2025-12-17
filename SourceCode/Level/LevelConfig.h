#ifndef LEVEL_CONFIG_H_INCLUDED
#define LEVEL_CONFIG_H_INCLUDED

#include <vector>
#include <string>

enum class MonsterType;

struct LevelConfig {
    int id;                        // 0/1/2/3
    std::string background_path;   // 背景路徑
    int spawn_rate;                // 生怪間隔
    std::vector<int> monsters;     // num_of_monsters（index=MonsterType）
};

// forward declare each maker
LevelConfig make_level0();
LevelConfig make_level1();
LevelConfig make_level2();
LevelConfig make_level3();

LevelConfig make_level_config(int lvl);

#endif
