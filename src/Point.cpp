#include <math.h>
#include <float.h>
#include <algorithm>
#include "GraphicPrinter.h"
#include "Point.h"
#include "Color.h"
#include "Services.h"

Point::Point() {
    this->x = 0;
    this->y = 0;
    this->snapTo = NULL;
}

Point::Point(const Point & _rf) {
    this->x = _rf.x;
    this->y = _rf.y;
    this->meta = _rf.meta;
    this->snapTo = NULL;
}

Point::Point(double _x, double _y) {
    this->x = _x;
    this->y = _y;
    this->snapTo = NULL;
}

Point::Point(int _x, int _y) {
    this->x = _x;
    this->y = _y;
    this->snapTo = NULL;
}

Point::~Point() {
}

/**
* draw visualized point as a cross (mostly for debugging)
*/
void Point::DrawVisualized(GraphicPrinter & printer, int flag) {

    int ptX = (int)this->x;
    int ptY = (int)this->y;
    int sz = 5;

    Color colA(255, 255, 255);

    printer.drawLine(colA, ptX, ptY - sz, ptX, ptY + sz);
    printer.drawLine(colA, ptX - sz, ptY, ptX + sz, ptY);

    sz = 2;

    Color colB(255, 0, 0);

    printer.drawLine(colB, ptX, ptY - sz, ptX, ptY + sz);
    printer.drawLine(colB, ptX - sz, ptY, ptX + sz, ptY);

}

double Point::getX() const {
    return this->x;
}

double Point::getY() const {
    return this->y;
}

int Point::getIntX() const {
    return (int)round(this->x);
}

int Point::getIntY() const {
    return (int)round(this->y);
}

void Point::setVals(double _x, double _y) {
    this->x = _x;
    this->y = _y;
}

void Point::setVals(const Point & _rf) {
    this->x = _rf.x;
    this->y = _rf.y;
}

void Point::addVals(double _x, double _y) {
    this->x += _x;
    this->y += _y;
}

/**
* step from Point by angle and radius
*/
void Point::addRadius(double _angle, double _radius) {
    this->x += sin(_angle) * _radius;
    this->y += cos(_angle) * _radius;
}

void Point::setX(double _x) {
    this->x = _x;
}

void Point::setY(double _y) {
    this->y = _y;
}

void Point::setMeta(double _meta) {
    this->meta = _meta;
}

double Point::getMeta() const {
    return this->meta;
}

/**
* @return return distance to Point
*/
double Point::distTo(Point & _rf) {
    return sqrt(pow(this->x - _rf.x, 2) + pow(this->y - _rf.y, 2));
}

/**
* @return return distance to Point specified by x, y
*/
double Point::distTo(double _x, double _y) {
    return sqrt(pow(this->x - _x, 2) + pow(this->y - _y, 2));
}

/**
* set snapped point - in graphical representation two snapped points are joined by line
* cyclic or finite snapped points may represent graphical object
*/
void Point::setSnapTo(Point * _snapTo) {
    this->snapTo = _snapTo;
}

Point * Point::getSnapTo() const {
    return this->snapTo;
}

/**
* determine if line from this point colliding with another line
* @param _ptTo Point representing direction from this point as a ray
* @param _colFrom Point of start of testing line
* @param _colTo Point of end of testing line
* @return returns true if collision happens
*/
bool Point::collideLineRay(const Point & _ptTo, const Point & _colFrom, const Point & _colTo) {

    // offsets

    double o1 = this->x;
    double o2 = this->y;
    double o3 = _colFrom.x;
    double o4 = _colFrom.y;

    // differences

    double d1 = _ptTo.x - this->x;
    double d2 = _ptTo.y - this->y;
    double d3 = _colTo.x - _colFrom.x;
    double d4 = _colTo.y - _colFrom.y;

    // parameter

    double kDiv = (d2*d3 - d4*d1);

    if (fabs(kDiv) < FLT_EPSILON) {
        return false;
    }

    double k = (o4*d1 - o2*d1 - d2*o3 + d2*o1) / kDiv;

    if (k > FLT_EPSILON && k <= 1.0 + FLT_EPSILON) {
        // collision coords (must be on line)

        Point col(o3 + d3*k, o4 + d4*k);

        // test if collision is on positive part on ray

        double rayK;

        if (fabs(d1) > fabs(d2)) {
            rayK = (col.x - o1) / d1;

        } else {
            rayK = (col.y - o2) / d2;
        }

        return rayK >= 0;
    }

    return false;
}

/**
* determine if line from this point colliding with another line and append collision result to vector
* sorted by the collision distance at ray line
* @param _ptTo Point representing direction from this point as a ray
* @param _colFrom Point of start of testing line
* @param _colTo Point of end of testing line
* @return returns true if some collision happens
*/
bool Point::collideLineAppendSorted(const Point & _ptTo, const Point & _colFrom, const Point & _colTo, std::vector<Point> * _appendTo) {

    // offsets

    double o1 = this->x;
    double o2 = this->y;
    double o3 = _colFrom.x;
    double o4 = _colFrom.y;

    // differences

    double d1 = _ptTo.x - this->x;
    double d2 = _ptTo.y - this->y;
    double d3 = _colTo.x - _colFrom.x;
    double d4 = _colTo.y - _colFrom.y;

    // parameter

    double kDiv = (d2*d3 - d4*d1);

    if (fabs(kDiv) < FLT_EPSILON) {
        return false;
    }

    double k = (o4*d1 - o2*d1 - d2*o3 + d2*o1) / kDiv;

    //if (k > 0 && k <= 1.0) {
    if (k > FLT_EPSILON && k <= 1.0 + FLT_EPSILON) {

        //std::cout << "K: " << k << " | " << fabs(kDiv) << "\n";

        // collision coords (must be on line)

        Point col(o3 + d3*k, o4 + d4*k);

        // store koef on ray line

        if (fabs(d1) > fabs(d2)) {
            col.setMeta((col.x - o1) / d1);

        } else {
            col.setMeta((col.y - o2) / d2);
        }

        std::vector<Point>::iterator _it = upper_bound(_appendTo->begin(), _appendTo->end(), col, [](const Point & ptA, const Point & ptB) {
            return ptA.getMeta() - ptB.getMeta() <= FLT_EPSILON;
        });

        if (_it == _appendTo->end()) {
            _appendTo->push_back(col);

        } else {
            _appendTo->insert(_it, col);
        }

        return true;
    }

    return false;
}
