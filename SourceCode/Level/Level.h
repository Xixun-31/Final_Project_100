#ifndef LEVEL_H_INCLUDED
#define LEVEL_H_INCLUDED

#include <vector>
#include "../shapes/Rectangle.h"
#include "../shapes/Point.h"

// 前向宣告，避免多餘 include
struct ALLEGRO_BITMAP;
enum class MonsterType;

/**
 * @brief The class manages data of each level.
 * @details 負責：載入關卡背景、設定怪物數量、控制何時生怪、畫出背景。
 */

struct SpawnUnit {
    MonsterType type;
    int count;
    int burst = 1;         // 每次 spawn 幾隻（burst=5 就是一幀生5隻）
    int spawnPointId = -1; // 從哪個 spawn point 出現，-1 表示隨機
};

struct Wave {
    std::vector<SpawnUnit> units;  // 這波要生什麼怪
    int spawn_interval = 60;       // 每幾幀生一隻
    int start_delay = 0;           // 進 wave 前延遲幾幀（可選）
    bool wait_until_clear = true;  // 生完後要等場上怪清空才進下一波
};

class Level
{
public:
    Level() {}

    void init();
    void load_level(int lvl);
    void update();
    void draw();
    bool all_waves_done() const;
    int remain_monsters() const {
        int res = 0;
        for (const int &i : num_of_monsters) res += i;
        return res;
    }

private:
    /**
     * @brief 目前關卡編號（0,1,2,3...）
     */
    int level = -1;

    /**
     * @brief 下一隻怪還要等多少 frame 才會生出來
     */
    int monster_spawn_counter = 0;

    /**
     * @brief 各種怪物還剩幾隻沒生出來
     *        index 對應 MonsterType 的 enum 值
     */
    std::vector<int> num_of_monsters;

    /**
     * @brief 關卡背景圖
     */
    ALLEGRO_BITMAP *background = nullptr;
    struct SpawnPoint {
        Point pos;
    };

    std::vector<SpawnPoint> spawn_points;

    std::vector<Wave> waves;
    int wave_idx = 0;

    int start_delay_counter = 0;

    size_t unit_idx = 0;        // wave 裡第幾種怪
    int unit_left = 0;          // 這種怪還剩幾隻沒生

};

#endif
