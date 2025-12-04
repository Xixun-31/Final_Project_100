#include "Effect.h"
#include "data/DataCenter.h"
#include "data/GIFCenter.h"
#include <allegro5/allegro.h>
#include "algif5/algif.h"  


void Effect::emit_split(const Point& pos) {
    DataCenter* DC = DataCenter::get_instance();

    EffectEvent ee;
    ee.type = EffectType::SPLIT;
    ee.pos = pos;
    ee.startTime = al_get_time();
    ee.life = 0.2; // 特效持續時間（秒）

    DC->effectEvents.push_back(ee);
}

// 每幀畫出特效，超過 life 秒就刪掉
void Effect::draw_all() {
    DataCenter *DC = DataCenter::get_instance();
    GIFCenter  *GC = GIFCenter::get_instance();

    auto &events = DC->effectEvents;

    for (auto it = events.begin(); it != events.end(); ) {
        EffectEvent &ee = *it;
        bool remove_this = false;

        switch (ee.type) {
            case EffectType::SPLIT: {
                // 從 GIFCenter 取得動畫（建議 GIFCenter 內部有快取）
                ALGIF_ANIMATION *anim = GC->get("assets/gif/effects/split_effect.gif");

                double t = al_get_time() - ee.startTime;
                ALLEGRO_BITMAP *frame = algif_get_bitmap(anim, t);

                if (frame) {
                    int w = al_get_bitmap_width(frame);
                    int h = al_get_bitmap_height(frame);
                    al_draw_bitmap(frame,
                                   ee.pos.x - w / 2,
                                   ee.pos.y - h / 2,
                                   0);
                }

                // 如果超過 life 秒，就不再保留這個 event
                if (t > ee.life) {
                    remove_this = true;
                }
                break;
            }

            default:
                break;
        }

        if (remove_this) {
            it = events.erase(it);
        } else {
            ++it;
        }
    }
}
