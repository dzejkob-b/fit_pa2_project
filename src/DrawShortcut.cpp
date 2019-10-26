#include "DrawShortcut.h"

DrawShortcut::DrawShortcut(DrawNode * _groupLink) {
    this->groupLink = _groupLink;
}

DrawShortcut::~DrawShortcut() {
}

int DrawShortcut::getType() const {
    return DrawNode::TYPE_SHORTCUT;
}

std::string DrawShortcut::getComposedText() const {
    std::stringstream str;

    str << "shortcut(" << this->groupLink->getNodeKey() << ", " << this->nodeKey << ", " << this->trans.getFixedOffsetX() << ", " << this->trans.getFixedOffsetY() << ")";

    return str.str();
}

DrawNode * DrawShortcut::getGroupLink() {
    return this->groupLink;
}

/**
* specific shortcut drawing logic - change translation of contained objects
* in linked group and render
*/
void DrawShortcut::shDraw(GraphicPrinter & print, DrawNode * _fromNd) {
    for (auto _it =_fromNd->getChilds().begin(); _it != _fromNd->getChilds().end(); ++_it) {

        DrawNode * loopNode = *_it;

        loopNode->setTransTempLink(this);
        loopNode->draw(print);
        loopNode->setTransTempLink(NULL);

        if (loopNode->haveChilds()) {
            this->shDraw(print, loopNode);
        }
    }
}

void DrawShortcut::draw(GraphicPrinter & print) {
    this->shDraw(print, this->groupLink);
}
