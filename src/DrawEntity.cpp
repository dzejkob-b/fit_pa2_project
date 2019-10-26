#include <sstream>
#include <math.h>
#include <iostream>
#include "DrawEntity.h"
#include "Point.h"
#include "Services.h"
#include "PointGenerator.h"

DrawEntity::DrawEntity(bool _isClosed) : filler(this) {
    this->isClosed = _isClosed;
}

DrawEntity::~DrawEntity() {
}

void DrawEntity::addPoint(Point _pt) {
    this->points.push_back(_pt);
}

void DrawEntity::addPointList(std::vector<Point> & _ls) {
    for (std::vector<Point>::iterator _it = _ls.begin(); _it != _ls.end(); ++_it) {
        this->points.push_back(*_it);
    }
}

int DrawEntity::getType() const {
    return DrawEntity::TYPE_ENTITY;
}

std::string DrawEntity::getTypeReadable(bool _details) const {
    std::stringstream str;

    str << DrawNode::getTypeReadable(_details);

    if (_details) {
        str << ", Points: " << this->points.size();

        if (!this->isClosed) {
            str << ", Unclosed";
        }
    }

    return str.str();
}

std::string DrawEntity::getComposedText() const {
    std::stringstream str;

    str << "polyline(" << this->points.size() << ", ";

    for (std::vector<Point>::const_iterator _it = this->points.begin(); _it != this->points.end(); ++_it) {
        str << (*_it).getX() << ", " << (*_it).getY() << ", ";
    }

    if (this->isClosed) {
        str << "closed, ";
    }

    str << this->filler.getColor().getFlag() << ", " << FillStyle::getFlagFromStyle(this->filler.getStyle()) << ")";

    return str.str();
}

DrawFiller * DrawEntity::getFiller() {
    return &this->filler;
}

bool DrawEntity::isInside(Point & _pt) {

    Point ptAddX(_pt);
    Point ptAddY(_pt);

    ptAddX.addVals(10, 0);
    ptAddY.addVals(0, 10);

    int colX = 0, colY = 0;

    Point uPtFrom;
    Point uPtTo;

    for (std::vector<Point>::iterator _it = this->points.begin(); _it != this->points.end(); ++_it) {

        Point * ptFrom = &(*_it);
        Point * ptTo = ptFrom->getSnapTo();

        if (ptTo != NULL) {
            uPtFrom.setVals(*ptFrom);
            uPtTo.setVals(*ptTo);

            this->applyTranslationChain(uPtFrom);
            this->applyTranslationChain(uPtTo);

            if (_pt.collideLineRay(ptAddX, uPtFrom, uPtTo)) {
                colX++;
            }

            if (_pt.collideLineRay(ptAddY, uPtFrom, uPtTo)) {
                colY++;
            }
        }
    }

    if (colX % 2 == 0 || colY % 2 == 0) {
        return false;

    } else {
        return true;
    }
}

std::vector<Point> DrawEntity::collidePoints(Point & ptRay, Point & ptDir) {

    std::vector<Point> result;

    Point uPtFrom;
    Point uPtTo;

    for (std::vector<Point>::iterator _it = this->points.begin(); _it != this->points.end(); ++_it) {

        Point * ptFrom = &(*_it);
        Point * ptTo = ptFrom->getSnapTo();

        if (ptTo != NULL) {
            uPtFrom.setVals(*ptFrom);
            uPtTo.setVals(*ptTo);

            this->applyTranslationChain(uPtFrom);
            this->applyTranslationChain(uPtTo);

            ptRay.collideLineAppendSorted(ptDir, uPtFrom, uPtTo, &result);
        }
    }

    return result;
}

DrawNode * DrawEntity::getClone() {
    DrawEntity * nRef = new DrawEntity(this->isClosed);

    nRef->trans = Translate(this->trans);
    nRef->filler.setStyle(this->filler.getStyle());
    nRef->filler.setColor(this->filler.getColor());

    for (auto _it = this->points.begin(); _it != this->points.end(); ++_it) {
        nRef->points.push_back(*_it);
    }

    return nRef;
}

Point DrawEntity::i_getLeftTop() {
    Point pt(this->minLeftTop);

    this->applyTranslationChain(pt);

    return pt;
}

Point DrawEntity::i_getRightTop() {
    Point pt(this->maxRightBottom.getX(), this->minLeftTop.getY());

    this->applyTranslationChain(pt);

    return pt;
}

Point DrawEntity::i_getLeftBottom() {
    Point pt(this->minLeftTop.getX(), this->maxRightBottom.getY());

    this->applyTranslationChain(pt);

    return pt;
}

Point DrawEntity::i_getRightBottom() {
    Point pt(this->maxRightBottom);

    this->applyTranslationChain(pt);

    return pt;
}

void DrawEntity::applyTranslation() {
    for (std::vector<Point>::iterator _it = this->points.begin(); _it != this->points.end(); ++_it) {
        this->trans.translate(*_it);
    }

    this->trans.reset();
}

void DrawEntity::rebuild() {

    // allocate points continuity

    PointGenerator p;

    p.buildContinuousSnaps(this->points, this->isClosed);

    // allocate bounding box

    bool st = true;

    for (std::vector<Point>::iterator _it = this->points.begin(); _it != this->points.end(); ++_it) {
        if (st) {
            this->minLeftTop = Point(*_it);
            this->maxRightBottom = Point(*_it);

            st = false;

        } else {
            if ((*_it).getX() < this->minLeftTop.getX()) this->minLeftTop.setX((*_it).getX());
            if ((*_it).getY() < this->minLeftTop.getY()) this->minLeftTop.setY((*_it).getY());

            if ((*_it).getX() > this->maxRightBottom.getX()) this->maxRightBottom.setX((*_it).getX());
            if ((*_it).getY() > this->maxRightBottom.getY()) this->maxRightBottom.setY((*_it).getY());
        }
    }
}

void DrawEntity::draw(GraphicPrinter & print) {

    this->drawFill(print);

    Point uPtFrom;
    Point uPtTo;

    Color col(200, 200, 200);

    for (std::vector<Point>::iterator _it = this->points.begin(); _it != this->points.end(); ++_it) {

        Point * ptFrom = &(*_it);
        Point * ptTo = ptFrom->getSnapTo();

        if (ptTo != NULL) {
            uPtFrom.setVals(*ptFrom);
            uPtTo.setVals(*ptTo);

            this->applyTranslationChain(uPtFrom);
            this->applyTranslationChain(uPtTo);

            print.drawLine(col, uPtFrom, uPtTo);
        }
    }
}

void DrawEntity::drawFill(GraphicPrinter & print) {
    if (this->isClosed) {

        this->filler.drawFill(print);

    }
}
