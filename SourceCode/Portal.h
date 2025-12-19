#ifndef PORTAL_H_INCLUDED
#define PORTAL_H_INCLUDED

#include "Object.h"
#include <string>

class Portal : public Object {
public:
    Portal(double x, double y, const char* img_path = "./assets/image/circle.png");
    void init();
    void update();
    void draw() override;
    
private:
    std::string img_path;
};

#endif
