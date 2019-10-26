#include "DrawCircle.h"
#include "FillStyle.h"

DrawCircle::DrawCircle(Point _center, double _radius, int _density) : DrawEllipse(_center, _radius, _radius, _density) {
}

DrawCircle::~DrawCircle() {
}

int DrawCircle::getType() const {
    return DrawEntity::TYPE_CIRCLE;
}

std::string DrawCircle::getComposedText() const {
    std::stringstream str;

    str << "circle(" << this->center.getX() << ", " << this->center.getY() << ", " << this->radiusX << ", " << this->density << ", ";
    str << this->filler.getColor().getFlag() << ", " << FillStyle::getFlagFromStyle(this->filler.getStyle()) << ")";

    return str.str();
}

DrawNode * DrawCircle::getClone() {
    DrawCircle * nRef = new DrawCircle(this->center, this->radiusX, this->density);

    nRef->trans = Translate(this->trans);
    nRef->filler.setStyle(this->filler.getStyle());
    nRef->filler.setColor(this->filler.getColor());

    return nRef;
}
