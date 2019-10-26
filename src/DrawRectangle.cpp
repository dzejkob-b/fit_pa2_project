#include "DrawRectangle.h"
#include "PointGenerator.h"
#include <iostream>

DrawRectangle::DrawRectangle(Point _st, double _width, double _height) : DrawEntity(true) {
    this->st = _st;
    this->width = _width;
    this->height = _height;

    if (this->width < 0) {
        this->st.setX(this->st.getX() + this->width);
        this->width *= -1;
    }

    if (this->height < 0) {
        this->st.setY(this->st.getY() + this->height);
        this->height *= -1;
    }
}

DrawRectangle::~DrawRectangle() {
}

int DrawRectangle::getType() const {
    return DrawEntity::TYPE_RECTANGLE;
}

std::string DrawRectangle::getComposedText() const {
    std::stringstream str;

    str << "rectangle(" << this->st.getX() << ", " << this->st.getY() << ", " << this->width << ", " << this->height << ", ";
    str << this->filler.getColor().getFlag() << ", " << FillStyle::getFlagFromStyle(this->filler.getStyle()) << ")";

    return str.str();
}

DrawNode * DrawRectangle::getClone() {
    DrawRectangle * nRef = new DrawRectangle(this->st, this->width, this->height);

    nRef->trans = Translate(this->trans);
    nRef->filler.setStyle(this->filler.getStyle());
    nRef->filler.setColor(this->filler.getColor());

    return nRef;
}

void DrawRectangle::applyTranslation() {

    this->trans.translate(this->st);
    this->trans.reset();

}

void DrawRectangle::rebuild() {

    this->points.clear();

    this->addPoint(this->st);
    this->addPoint(Point(this->st.getX() + this->width, this->st.getY()));
    this->addPoint(Point(this->st.getX() + this->width, this->st.getY() + this->height));
    this->addPoint(Point(this->st.getX(), this->st.getY() + this->height));

    DrawEntity::rebuild();
}
