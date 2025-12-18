  // Monster.h
  #ifndef MONSTER_H_INCLUDED
  #define MONSTER_H_INCLUDED

  #include "../shapes/Point.h"
  #include "../shapes/Rectangle.h"
  #include "../shapes/Circle.h"
  #include <vector>

  class DataCenter;

enum class MonsterType {
  WOLF,
  SMALLWOLF,
  CAVEMAN,
  WOLFKNIGHT,
  DEMONNINJA,
  BIRD,
  ELITE,
  SLIME,
  SPLIT,
  SUICIDE,
  BARREL,
  MONSTERTYPE_MAX
};

  enum class Dir { UP, DOWN, LEFT, RIGHT };

  #include "../Object.h"

  class Monster : public Object {
  public:
    static Monster *create_monster(MonsterType type, const Point &p);

    virtual ~Monster() {}

    virtual void update();
    virtual void draw();
    virtual int get_money() const;
    int money;
    int HP;
    virtual void special_ability(DataCenter* DC);
    MonsterType peek_type() const;
    void on_hit(const Point& from, double kb_strength);
    
  protected:
    // 建構子：不再吃 path，只記住自己是什麼 type
    Monster(MonsterType type, const Point &p);

    
    Dir dir;

    double v; // 速度：像素 / 秒（在子類別設定）
    int bitmap_img_id;
    int bitmap_switch_counter;
    int bitmap_switch_freq;
    std::vector<int> bitmap_img_ids[4];
    MonsterType type; 
    
    // hit
    double hit_flash_timer = 0.0;   // 秒
    double kb_vx = 0.0;             // 擊退速度 px/s
    double kb_vy = 0.0;
  };

  #endif
