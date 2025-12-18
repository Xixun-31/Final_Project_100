#include "Effect.h"
#include "data/DataCenter.h"
#include "data/GIFCenter.h"
#include "data/ImageCenter.h"
#include <allegro5/allegro.h>
#include "algif5/algif.h"  

namespace {
    const int DEATH_FRAMES = 4; 
}

void Effect::emit_split(const Point& pos) {
    DataCenter* DC = DataCenter::get_instance();

    EffectEvent ee;
    ee.type = EffectType::SPLIT;
    ee.pos = pos;
    ee.startTime = al_get_time();
    ee.life = 1.0; // 特效持續時間（秒）

    DC->effectEvents.push_back(ee);
}

void Effect::emit_slime_death(const Point& pos) {
    
    EffectEvent ee;
    ee.type = EffectType::SLIME_DEATH;
    ee.pos = pos;
    ee.startTime = al_get_time();
    ee.frame = 0;
    ee.life = 0.12;     // 每幀 0.12 秒

    DataCenter::get_instance()->effectEvents.push_back(ee);
}

// for caveman death effect
void Effect::emit_SSR_death(const Point& pos) {
    
    EffectEvent ee;
    ee.type = EffectType::SSR_DEATH;
    ee.pos = pos;
    ee.startTime = al_get_time();
    ee.frame = 0;
    ee.life = 0.12;     // 每幀 0.12 秒

    DataCenter::get_instance()->effectEvents.push_back(ee);
}

void Effect::emit_death(const Point& pos) {
    DataCenter* DC = DataCenter::get_instance();

    EffectEvent ee;
    ee.type = EffectType::DEATH;
    ee.pos = pos;
    ee.startTime = al_get_time();
    ee.life = 0.5; // 特效持續時間（秒）

    DC->effectEvents.push_back(ee);
}



void Effect::emit_explosion(const Point& pos) {
    DataCenter* DC = DataCenter::get_instance();

    EffectEvent ee;
    ee.type = EffectType::EXPLOSION;
    ee.pos = pos;
    ee.startTime = al_get_time();
    ee.frame = 0;
    ee.life = 0.5; // Roughly enough for 8 frames if fast, or adjust based on frames
    // 8 frames, maybe 0.1s per frame? total 0.8s. Let's strictly control via frame count
    
    DC->effectEvents.push_back(ee);
}



// 每幀畫出特效，超過 life 秒就刪掉
void Effect::draw_all() {
    DataCenter *DC = DataCenter::get_instance();
    GIFCenter  *GC = GIFCenter::get_instance();
    ImageCenter *IC = ImageCenter::get_instance();

    auto &events = DC->effectEvents;

    for (auto it = events.begin(); it != events.end(); ) {
        EffectEvent &ee = *it;
        bool remove_this = false;
        double t = al_get_time() - ee.startTime;
        
    
        switch (ee.type) {
            case EffectType::SPLIT: {
                // 從 GIFCenter 取得動畫（建議 GIFCenter 內部有快取）
                ALGIF_ANIMATION *anim = GC->get("assets/gif/effects/effect_split.gif");

                if (!anim) break;

                // Calculate actual total duration of the GIF
                double total_duration = 0.0;
                for (int i = 0; i < anim->frames_count; ++i) {
                     total_duration += algif_get_frame_duration(anim, i);
                }

                ALLEGRO_BITMAP *frame = algif_get_bitmap(anim, t);
                if (frame) {
                    int w = al_get_bitmap_width(frame);
                    int h = al_get_bitmap_height(frame);
                    al_draw_bitmap(frame,
                                   ee.pos.x - w / 2,
                                   ee.pos.y - h / 2,
                                   0);
                }
                // Play exactly once
                if (t >= total_duration) {
                    remove_this = true;
                }
                break;
            }
            case EffectType::SSR_DEATH: {
                ee.frame = static_cast<int>(t / ee.life);

            
                if (ee.frame >= DEATH_FRAMES) {
                    remove_this = true;
                    break;
                }

                std::string path = "assets/image/effects/DEATH_" + std::to_string(ee.frame) + ".png";
                ALLEGRO_BITMAP* bmp = IC->get(path);  // 這裡如果路徑錯，GAME_ASSERT 會直接把你殺掉

                if (bmp) {
                    int w = al_get_bitmap_width(bmp);
                    int h = al_get_bitmap_height(bmp);
                    al_draw_bitmap(bmp, ee.pos.x - w/2, ee.pos.y - h/2, 0);
                }
                break;
            }
            case EffectType::SLIME_DEATH: {
                ee.frame = static_cast<int>(t / ee.life);

            
               if (!(ee.life > 0.0) || std::isnan(ee.life) || std::isinf(ee.life) || ee.frame > 6) {
                    // debug print
                    printf("[Effect] bad life=%f type=%d start=%f now=%f\n",
                        ee.life, (int)ee.type, ee.startTime, al_get_time());
                        remove_this = true;
                    break;
                }

                std::string path = "assets/image/monster/Slime/DEATH_" + std::to_string(ee.frame) + ".png";
                ALLEGRO_BITMAP* bmp = IC->get(path);  
                if (bmp) {
                    int w = al_get_bitmap_width(bmp);
                    int h = al_get_bitmap_height(bmp);
                    al_draw_bitmap(bmp, ee.pos.x - w/2, ee.pos.y - h/2, 0);
                }
                break;
            }
            case EffectType::DEATH: {
                // Currently empty/commented out. 
                // If it falls through, it hits EXPLOSION.
                // Added break to prevent fallthrough.
                break;
            }
            case EffectType::EXPLOSION: {
                 // 8 frames: explosion_1 ~ explosion_8
                 // duration per frame: 0.1s => total 0.8s
                 double duration_per_frame = 0.1;
                 int total_frames = 8;
                 
                 ee.frame = static_cast<int>(t / duration_per_frame);
                 
                 if (ee.frame >= total_frames) {
                     remove_this = true;
                     break;
                 }
                 
                 std::string path = "assets/image/monster/barrel/explosion_" + std::to_string(ee.frame + 1) + ".png";
                 ALLEGRO_BITMAP* bmp = IC->get(path);
                 
                 if (bmp) {
                     int w = al_get_bitmap_width(bmp);
                     int h = al_get_bitmap_height(bmp);
                     al_draw_bitmap(bmp, ee.pos.x - w/2, ee.pos.y - h/2, 0);
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
