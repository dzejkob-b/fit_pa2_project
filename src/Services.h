#ifndef SERVICES_H_INCLUDED
#define SERVICES_H_INCLUDED

#include "IRenderer.h"
#include "IMouse.h"

/**
* public single instance for sharing necessary resources
*/
class Services {
    private:
        static Services sRef;

        IRenderer * rnd = NULL;
        IMouse * mouse = NULL;

    public:
        static Services * getRef();

        void setRnd(IRenderer * _rnd);
        IRenderer * getRnd();

        void setMouse(IMouse * _mouse);
        IMouse * getMouse();
};

#endif // SERVICES_H_INCLUDED
