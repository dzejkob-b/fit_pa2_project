#ifndef IMOUSE_H_INCLUDED
#define IMOUSE_H_INCLUDED

#include <SDL2/SDL.h>

/**
* interface providing mouse position and state
*/
class IMouse {
    public:

        virtual ~IMouse() {}
        virtual int i_getMouseX() = 0;
        virtual int i_getMouseY() = 0;
        virtual int i_getMouseButton() = 0;
        virtual SDL_Event i_getEvent() = 0;
        virtual void i_setEventUsed() = 0;

};

#endif // IMOUSE_H_INCLUDED
