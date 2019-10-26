#ifndef TRANSLATE_H_INCLUDED
#define TRANSLATE_H_INCLUDED

#include "Point.h"

/**
* class representing translation by delta x, y (translations may be chained)
*/
class Translate {
    private:
        double fixedOffsetX;
        double fixedOffsetY;

        double offsetX;
        double offsetY;

    public:
        Translate();
        Translate(const Translate & _tr);
        ~Translate();

        double getOffsetX() const;
        double getOffsetY() const;

        double getFixedOffsetX() const;
        double getFixedOffsetY() const;

        void addOffset(double _x, double _y);
        void setOffset(double _x, double _y);

        void translate(Point & _pt);
        void translate(Translate & _tr);

        void translateRevert(Point & _pt);
        void translateRevert(Translate & _tr);

        void applyFixed();
        void reset();
};

#endif // TRANSLATE_H_INCLUDED
