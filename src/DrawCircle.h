#ifndef DRAWCIRCLE_H_INCLUDED
#define DRAWCIRCLE_H_INCLUDED

#include "DrawEntity.h"
#include "DrawEllipse.h"

/**
* simplier circle class inherited from ellipse (only one radius dimension)
*/
class DrawCircle : public DrawEllipse {
    private:
    public:
        DrawCircle(Point _center, double _radius, int _density);
        ~DrawCircle();

        int getType() const;
        std::string getComposedText() const;

        virtual DrawNode * getClone();
};

#endif // DRAWCIRCLE_H_INCLUDED
