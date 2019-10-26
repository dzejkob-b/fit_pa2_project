#ifndef RECT_H_INCLUDED
#define RECT_H_INCLUDED

#include "SDL2/SDL.h"

/**
* base rectangle class
*/
class Rect {
    public:
        double x = 0;
        double y = 0;
        double w = 0;
        double h = 0;

        void setSDLRect(SDL_Rect & re);
};

#endif // RECT_H_INCLUDED
