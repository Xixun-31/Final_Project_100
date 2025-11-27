// Monster.h
#ifndef MONSTER_H_INCLUDED
#define MONSTER_H_INCLUDED

#include "../shapes/Point.h"
#include "../shapes/Rectangle.h"
#include <vector>


enum class MonsterType {
  WOLF,
  CAVEMAN,
  WOLFKNIGHT,
  DEMONNIJIA,
  BIRD,
  ELITE,
  SLIME,
  SPLIT,
  SUICIDE,
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

protected:
  // 建構子：不再吃 path，只記住自己是什麼 type
  Monster(MonsterType type, const Point &p);

  MonsterType type;
  Dir dir;

  double v; // 速度：像素 / 秒（在子類別設定）
  int bitmap_img_id;
  int bitmap_switch_counter;
  int bitmap_switch_freq;
  std::vector<int> bitmap_img_ids[4];
  
};

#endif
