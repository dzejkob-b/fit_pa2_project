#include "Translate.h"
#include <iostream>

Translate::Translate() {
    this->fixedOffsetX = 0;
    this->fixedOffsetY = 0;

    this->offsetX = 0;
    this->offsetY = 0;
}

Translate::Translate(const Translate & _tr) {
    this->fixedOffsetX = _tr.fixedOffsetX;
    this->fixedOffsetY = _tr.fixedOffsetY;

    this->offsetX = _tr.offsetX;
    this->offsetY = _tr.offsetY;
}

Translate::~Translate() {
}

double Translate::getOffsetX() const {
    return this->offsetX;
}

double Translate::getOffsetY() const {
    return this->offsetY;
}

double Translate::getFixedOffsetX() const {
    return this->fixedOffsetX;
}

double Translate::getFixedOffsetY() const {
    return this->fixedOffsetY;
}

void Translate::addOffset(double _x, double _y) {
    this->offsetX += _x;
    this->offsetY += _y;
}

void Translate::setOffset(double _x, double _y) {
    this->offsetX = _x;
    this->offsetY = _y;
}

void Translate::translate(Point & _pt) {
    _pt.addVals(this->offsetX + this->fixedOffsetX, this->offsetY + this->fixedOffsetY);
}

void Translate::translate(Translate & _tr) {
    this->offsetX += _tr.offsetX + _tr.fixedOffsetX;
    this->offsetY += _tr.offsetY + _tr.fixedOffsetY;
}

void Translate::translateRevert(Point & _pt) {
    _pt.addVals(- (this->offsetX + this->fixedOffsetX), - (this->offsetY + this->fixedOffsetY));
}

void Translate::translateRevert(Translate & _tr) {
    this->offsetX -= _tr.offsetX + _tr.fixedOffsetX;
    this->offsetY -= _tr.offsetY + _tr.fixedOffsetY;
}

/**
* apply translation to be fixed (make motion permanent)
*/
void Translate::applyFixed() {
    this->fixedOffsetX += this->offsetX;
    this->fixedOffsetY += this->offsetY;

    this->offsetX = 0;
    this->offsetY = 0;
}

/**
* reset temporary translation (keeps fixed offsets untouched)
*/
void Translate::reset() {
    this->offsetX = 0;
    this->offsetY = 0;
}
