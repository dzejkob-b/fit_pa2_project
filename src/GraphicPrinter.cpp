#include "GraphicPrinter.h"
#include "Point.h"

GraphicPrinter::GraphicPrinter() {
}

GraphicPrinter::~GraphicPrinter() {
}

void GraphicPrinter::drawPixel(Color & col, Point & pt) {
}

void GraphicPrinter::drawLine(Color & col, Point & ptFrom, Point & ptTo) {

    // default simple put pixel implementation

    double dist = ptFrom.distTo(ptTo);
    double offX = (ptTo.getX() - ptFrom.getX()) / dist;
    double offY = (ptTo.getY() - ptFrom.getY()) / dist;

    double pos = 0;

    this->tmpPt.setVals(ptFrom);

    while (pos < dist) {

        this->drawPixel(col, this->tmpPt);

        this->tmpPt.addVals(offX, offY);

        pos++;
    }
}

void GraphicPrinter::drawLine(Color & col, double fromX, double fromY, double toX, double toY) {

    this->tmpFrom.setVals(fromX, fromY);
    this->tmpTo.setVals(toX, toY);

    this->drawLine(col, this->tmpFrom, this->tmpTo);
}

void GraphicPrinter::drawRectangle(Color & col, Rect & re) {

    this->drawLine(col, re.x, re.y, re.x + re.w, re.y);
    this->drawLine(col, re.x + re.w, re.y, re.x + re.w, re.y + re.h);
    this->drawLine(col, re.x + re.w, re.y + re.h, re.x, re.y + re.h);
    this->drawLine(col, re.x, re.y + re.h, re.x, re.y);

}

void GraphicPrinter::drawFilledRectangle(Color & col, Rect & re) {

    this->drawRectangle(col, re);

}
