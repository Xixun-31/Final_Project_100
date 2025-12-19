#ifndef MONSTERTREASURE_H_INCLUDED
#define MONSTERTREASURE_H_INCLUDED

#include "Monster.h"

class MonsterTreasure : public Monster {
public:
    MonsterTreasure(const Point &p);
    virtual void update() override;
    virtual void draw() override;
    
private:
    bool dying;
    int dying_counter;
};

#endif
