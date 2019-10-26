#include "DrawEllipse.h"
#include "PointGenerator.h"

DrawEllipse::DrawEllipse(Point _center, double _radiusX, double _radiusY, int _density) : DrawEntity(true) {
    this->center = _center;
    this->radiusX = _radiusX;
    this->radiusY = _radiusY;
    this->density = _density;
}

DrawEllipse::~DrawEllipse() {
}

int DrawEllipse::getType() const {
    return DrawEntity::TYPE_ELLIPSE;
}

std::string DrawEllipse::getComposedText() const {
    std::stringstream str;

    str << "ellipse(" << this->center.getX() << ", " << this->center.getY() << ", " << this->radiusX << ", " << this->radiusY << ", " << this->density << ", ";
    str << this->filler.getColor().getFlag() << ", " << FillStyle::getFlagFromStyle(this->filler.getStyle()) << ")";

    return str.str();
}

DrawNode * DrawEllipse::getClone() {
    DrawEllipse * nRef = new DrawEllipse(this->center, this->radiusX, this->radiusY, this->density);

    nRef->trans = Translate(this->trans);
    nRef->filler.setStyle(this->filler.getStyle());
    nRef->filler.setColor(this->filler.getColor());

    return nRef;
}

void DrawEllipse::applyTranslation() {

    this->trans.translate(this->center);
    this->trans.reset();

}

void DrawEllipse::rebuild() {

    this->points.clear();

    PointGenerator p;

    p.setDensity(this->density);
    p.createEllipse(this->center, this->radiusX, this->radiusY);
    p.appendTo(this->points);

    DrawEntity::rebuild();
}
