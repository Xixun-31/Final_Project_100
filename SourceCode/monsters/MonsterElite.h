#ifndef MONSTERELITE_H_INCLUDED
#define MONSTERELITE_H_INCLUDED

#include "Monster.h"
#include "../shapes/Point.h"
#include <array>
#include <allegro5/allegro.h>
#include <string>

class DataCenter;

// fixed settings: MonsterElite attributes
class MonsterElite : public Monster {
public:
  MonsterElite(const Point& p);
  void special_ability(DataCenter* DC) override;
  void update() override;
  void draw() override;
  bool is_moving = false;
  int maxHP;
  private:
    enum class BossMode {
        IDLE,     // 原地晃動
        MOVE,     // 慢慢移動
        DASH,     // 突然朝 hero 衝刺
        ULT_MOVE,
        ULT_CHARGE,
        ULT_FIRE       
    };

    // ===== 動畫相關 =====
    BossMode mode = BossMode::IDLE;

    std::array<std::string, 4> idle_img;  // 靜止 4 張
    std::array<std::string, 5> move_img;  // 移動 5 張
    std::array<std::string, 4> diag_bullet;  // 4 個方向子彈圖

    int   anim_frame       = 0;
    double anim_timer      = 0.0;
    double idle_frame_dt   = 0.18;  // 靜止張數切換間隔
    double move_frame_dt   = 0.12;  // 移動張數切換間隔

    // ===== 噴子彈相關 =====
    double last_shot_time  = 0.0;
    double shot_cooldown   = 0.30;  // 幾秒噴一顆

    // ===== 衝刺（dash）相關 =====
    double last_dash_time  = 0.0;
    double dash_cooldown   = 3.0;   // 幾秒才會發動一次衝刺
    double dash_duration   = 0.75;  // 衝刺持續多久
    double dash_timer      = 0.0;
    double dash_speed      = 260.0; // 衝刺速度
    int dash_count = 0;

    double vx = 0.0;  // 一般移動速度 x（MOVE 模式用）
    double vy = 0.0;  // 一般移動速度 y
    double dash_dir_x = 0.0; // 衝刺方向向量
    double dash_dir_y = 0.0;

    // ULT 參數
    double ult_timer = 0.0;
    double ult_move_speed = 520.0;     // 去中心速度
    double ult_duration = 3.0;         // 旋轉彈幕持續秒數
    double ult_shot_interval = 0.08;   // 發射間隔
    double ult_shot_timer = 0.0;

    double spin_angle = 0.0;           // 目前旋轉角
    double spin_speed = 4.0;           // 角速度（rad/s），越大轉越快
    // 蓄力
    double charge_timer = 0.0;
    double charge_duration = 0.8;

    // 震動/閃紅
    double ult_shake_phase = 0.0;   // 用來做抖動

    

    void _update_animation(DataCenter* DC);
    void _update_motion(DataCenter* DC);
    void _enter_dash_toward_hero(DataCenter* DC);
    void _fire_diagonal_barrage(DataCenter* DC);
    void _fire_spinning_barrage(DataCenter* DC);
    
};

#endif
