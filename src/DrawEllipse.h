#ifndef DRAWELLIPSE_H_INCLUDED
#define DRAWELLIPSE_H_INCLUDED

#include "DrawEntity.h"

/**
* specific ellipse class inherited from basic graphic entity
*/
class DrawEllipse : public DrawEntity {
    protected:
        Point center;

        double radiusX;
        double radiusY;
        int density;

    public:
        DrawEllipse(Point _center, double _radiusX, double _radiusY, int _density);
        ~DrawEllipse();

        int getType() const;
        std::string getComposedText() const;

        virtual DrawNode * getClone();

        void applyTranslation();
        void rebuild();
};

#endif // DRAWELLIPSE_H_INCLUDED
