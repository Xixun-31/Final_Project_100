#ifndef EFFECT_H_INCLUDED
#define EFFECT_H_INCLUDED

#include "shapes/Point.h"

// 特效種類
enum class EffectType {
    SPLIT,
    // 之後要加更多特效就加在這裡，例如：
    // EXPLOSION,
    // ICE,
    // HEAL,
};

// 一個特效事件：在某個位置，從某時刻開始，存活 life 秒
struct EffectEvent {
    EffectType type;
    Point pos;
    double startTime;  // 動畫開始時間（al_get_time()）
    double life;       // 這個特效要存在多久（秒）
};

namespace Effect {
    // 每幀畫出所有特效，並移除已經過期的
    void draw_all();

    // 產生一個「分裂」特效（狼分裂用）
    void emit_split(const Point& pos);
}

#endif
