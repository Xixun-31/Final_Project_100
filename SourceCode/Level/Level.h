#ifndef LEVEL_H_INCLUDED
#define LEVEL_H_INCLUDED

#include <vector>
#include "../shapes/Rectangle.h"

// 前向宣告，避免多餘 include
struct ALLEGRO_BITMAP;
enum class MonsterType;

/**
 * @brief The class manages data of each level.
 * @details 負責：載入關卡背景、設定怪物數量、控制何時生怪、畫出背景。
 */
class Level
{
public:
    Level() {}

    void init();
    void load_level(int lvl);
    void update();
    void draw();

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
};

#endif
