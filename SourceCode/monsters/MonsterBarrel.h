#ifndef MONSTERBARREL_H_INCLUDED
#define MONSTERBARREL_H_INCLUDED

#include "Monster.h"

class MonsterBarrel : public Monster {
public:
    MonsterBarrel(const Point &p);
    virtual void update() override;
    virtual void draw() override;
    
private:
    bool exploded;
};

#endif
