#include "PointGenerator.h"
#include <math.h>

PointGenerator::PointGenerator() {
}

PointGenerator::~PointGenerator() {
}

void PointGenerator::setDensity(int _density) {
    this->density = _density;

    if (this->density < 2) {
        this->density = 2;

    } else if (this->density > 200) {
        this->density = 200;
    }
}

/**
* build points by ellipse logic
*/
void PointGenerator::createEllipse(Point center, double radiusX, double radiusY) {

    this->points.clear();

    for (int sf = 0; sf < this->density; sf++) {
        double ang = (M_PI * 2.0 / this->density) * sf;

        Point nPt;

        nPt.setVals(center.getX() + sin(ang) * radiusX, center.getY() + cos(ang) * radiusY);
        this->points.push_back(nPt);
    }
}

/**
* append generated points to another vector
*/
void PointGenerator::appendTo(std::vector<Point> & trg) {

    int startIdx = trg.size();

    for (std::vector<Point>::iterator _it = this->points.begin(); _it != this->points.end(); ++_it) {
        trg.push_back(*_it);
    }

    this->buildContinuousSnaps(trg, true, startIdx);
}

/**
* build closed or unclosed point snaps
*/
void PointGenerator::buildContinuousSnaps(std::vector<Point> & trg, bool isClosed, int startIdx, int toIdx) {

    if (toIdx == -1) toIdx = trg.size() - 1;

    for (int sf = startIdx; sf < toIdx; sf++) {
        trg[sf].setSnapTo(&trg[sf + 1]);
    }

    if (isClosed) {
        trg[toIdx].setSnapTo(&trg[startIdx]);
    }
}
