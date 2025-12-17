#include "LevelConfig.h"



LevelConfig make_level_config(int lvl) {
    switch (lvl) {
    case 0: return make_level0();
    case 1: return make_level1();
    case 2: return make_level2();
    case 3: return make_level3();
    default: return make_level1();
    }
}
