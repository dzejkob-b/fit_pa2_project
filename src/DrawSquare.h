#ifndef DRAWSQUARE_H_INCLUDED
#define DRAWSQUARE_H_INCLUDED

#include "DrawEntity.h"
#include "DrawRectangle.h"

/**
* simplier square class inherited from rectangle (only one dimension)
*/
class DrawSquare : public DrawRectangle {
    private:
    public:
        DrawSquare(Point _st, double _size);
        ~DrawSquare();

        int getType() const;
        std::string getComposedText() const;

        virtual DrawNode * getClone();
};

#endif // DRAWSQUARE_H_INCLUDED
