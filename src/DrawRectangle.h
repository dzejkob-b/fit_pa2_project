#ifndef DRAWRECTANGLE_H_INCLUDED
#define DRAWRECTANGLE_H_INCLUDED

#include "DrawEntity.h"

/**
* specific rectangle class inherited from basic graphic entity
*/
class DrawRectangle : public DrawEntity {
    protected:
        Point st;

        double width;
        double height;

    public:
        DrawRectangle(Point _st, double _width, double _height);
        ~DrawRectangle();

        int getType() const;
        std::string getComposedText() const;

        virtual DrawNode * getClone();

        void applyTranslation();
        void rebuild();
};

#endif // DRAWRECTANGLE_H_INCLUDED
