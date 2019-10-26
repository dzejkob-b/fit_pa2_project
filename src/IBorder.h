#ifndef IBORDER_H_INCLUDED
#define IBORDER_H_INCLUDED

#include "Point.h"

/**
* interface providing graphical element border
*/
class IBorder {
    public:
        virtual Point i_getLeftTop() = 0;
        virtual Point i_getRightTop() = 0;
        virtual Point i_getLeftBottom() = 0;
        virtual Point i_getRightBottom() = 0;
};

#endif // IBORDER_H_INCLUDED
