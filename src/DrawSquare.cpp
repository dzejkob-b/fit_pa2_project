#include "DrawSquare.h"

DrawSquare::DrawSquare(Point _st, double _size) : DrawRectangle(_st, _size, _size) {
}

DrawSquare::~DrawSquare() {
}

int DrawSquare::getType() const {
    return DrawEntity::TYPE_SQUARE;
}

std::string DrawSquare::getComposedText() const {
    std::stringstream str;

    str << "square(" << this->st.getX() << ", " << this->st.getY() << ", " << this->width << ", ";
    str << this->filler.getColor().getFlag() << ", " << FillStyle::getFlagFromStyle(this->filler.getStyle()) << ")";

    return str.str();
}

DrawNode * DrawSquare::getClone() {
    DrawSquare * nRef = new DrawSquare(this->st, this->width);

    nRef->trans = Translate(this->trans);
    nRef->filler.setStyle(this->filler.getStyle());
    nRef->filler.setColor(this->filler.getColor());

    return nRef;
}
