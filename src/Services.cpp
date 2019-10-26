#include "Services.h"

Services Services::sRef;

Services * Services::getRef() {
    return &Services::sRef;
}

void Services::setRnd(IRenderer * _rnd) {
    this->rnd = _rnd;
}

IRenderer * Services::getRnd() {
    return this->rnd;
}

void Services::setMouse(IMouse * _mouse) {
    this->mouse = _mouse;
}

IMouse * Services::getMouse() {
    return this->mouse;
}
