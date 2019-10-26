#include "UserDrawHelp.h"
#include "UserControl.h"
#include "Services.h"
#include "PointGenerator.h"
#include "DrawEntity.h"
#include "DrawEllipse.h"
#include "DrawCircle.h"
#include "DrawRectangle.h"
#include "DrawSquare.h"
#include <math.h>

UserDrawHelp::UserDrawHelp() {

}

UserDrawHelp::~UserDrawHelp() {

}

void UserDrawHelp::setState(int _newState, DrawNode * _underGroupNode) {
    this->state = _newState;

    this->underGroupNode = _underGroupNode;
}

void UserDrawHelp::addDensity(int _toAdd) {
    this->useDensity += _toAdd;

    if (this->useDensity < 3) {
        this->useDensity = 3;
    }
}

bool UserDrawHelp::havePoint() {
    return this->points.size() > 0;
}

void UserDrawHelp::addPoint(Point _pt) {
    this->points.push_back(_pt);
}

void UserDrawHelp::setLinePoint(Point _pt) {

    if (this->points.size() == 1) {
        this->points.push_back(Point(_pt));
    }

    std::vector<Point>::iterator _it = this->points.begin();

    _it++;
    (*_it).setVals(_pt);
    (*this->points.begin()).setSnapTo(&(*_it));
}

void UserDrawHelp::setRectanglePoint(Point _pt, bool _sqare) {

    if (this->points.size() == 1) {
        this->points.push_back(Point(_pt));
        this->points.push_back(Point(_pt));
        this->points.push_back(Point(_pt));
    }

    this->width = _pt.getX() - (*this->points.begin()).getX();
    this->height = _pt.getY() - (*this->points.begin()).getY();

    if (_sqare) {
        if (abs(width) < abs(height)) {
            height = abs(width) * (height < 0 ? -1 : 1);

        } else {
            width = abs(height) * (width < 0 ? -1 : 1);
        }
    }

    std::vector<Point>::iterator _it = this->points.begin();
    std::vector<Point>::iterator _itBef = this->points.begin();

    this->ptX = (*_it).getX();
    this->ptY = (*_it).getY();

    _it++;
    (*_it).setVals(this->ptX + this->width, this->ptY);
    (*_itBef).setSnapTo(&(*_it));
    _itBef++;

    _it++;
    (*_it).setVals(this->ptX + this->width, this->ptY + this->height);
    (*_itBef).setSnapTo(&(*_it));
    _itBef++;

    _it++;
    (*_it).setVals(this->ptX, this->ptY + this->height);
    (*_itBef).setSnapTo(&(*_it));
    _itBef++;

    (*_itBef).setSnapTo(&(*this->points.begin()));

    if (this->state == UserControl::STATE_CIRCLE || this->state == UserControl::STATE_ELLIPSE) {
        // add preview circle / ellpise

        while (this->points.size() > 4) {
            this->points.pop_back();
        }

        PointGenerator pg;

        pg.setDensity(this->useDensity);
        pg.createEllipse(Point(this->ptX + this->width / 2.0, this->ptY + this->height / 2.0), abs(this->width) / 2.0, abs(this->height) / 2.0);
        pg.appendTo(this->points);

    }
}

void UserDrawHelp::updatePreview() {

}

void UserDrawHelp::clearPoints(bool _keepLast) {
    if (this->points.size() > 0 && _keepLast) {
        Point pt(*(--this->points.end()));

        this->points.clear();
        this->points.push_back(pt);

    } else {
        this->points.clear();
    }
}

void UserDrawHelp::resetClean() {
    this->state = UserControl::STATE_NONE;

    this->points.clear();
    this->finalPoints.clear();
}

void UserDrawHelp::stackFinalPoints() {

    std::vector<Point>::iterator _it = this->points.begin();

    PointGenerator pg;

    switch (this->state) {
        case UserControl::STATE_POLYLINE :
            if (this->points.size() > 0) {

                if (this->finalPoints.size() == 0) {
                    this->finalPoints.push_back(*_it);
                    _it++;
                    this->finalPoints.push_back(*_it);

                } else {
                    _it++;
                    this->finalPoints.push_back(*_it);
                }

                pg.buildContinuousSnaps(this->finalPoints, false);

            }
            break;

        case UserControl::STATE_SQUARE :
        case UserControl::STATE_RECTANGLE :
            this->finalPoints.clear();

            for (_it = this->points.begin(); _it != this->points.end(); ++_it) {
                this->finalPoints.push_back(*_it);
            }

            pg.buildContinuousSnaps(this->finalPoints, true);

            break;

        case UserControl::STATE_CIRCLE :
        case UserControl::STATE_ELLIPSE :

            this->finalPoints.clear();

            pg.setDensity(this->useDensity);
            pg.createEllipse(Point(this->ptX + this->width / 2.0, this->ptY + this->height / 2.0), abs(this->width) / 2.0, abs(this->height) / 2.0);
            pg.appendTo(this->finalPoints);

            break;
    }
}

void UserDrawHelp::pushToDrawArea(DrawArea * _area, bool _closed) {

    DrawEntity * ent = NULL;

    switch (this->state) {
        case UserControl::STATE_POLYLINE :
            ent = new DrawEntity(_closed);

            ent->addPointList(this->finalPoints);
            break;

        case UserControl::STATE_SQUARE :
            ent = new DrawSquare(Point(this->ptX, this->ptY), this->width);
            break;

        case UserControl::STATE_RECTANGLE :
            ent = new DrawRectangle(Point(this->ptX, this->ptY), this->width, this->height);
            break;

        case UserControl::STATE_CIRCLE :
            ent = new DrawCircle(Point(this->ptX + this->width / 2.0, this->ptY + this->height / 2.0), abs(this->width) / 2.0, this->useDensity);
            break;

        case UserControl::STATE_ELLIPSE :
            ent = new DrawEllipse(Point(this->ptX + this->width / 2.0, this->ptY + this->height / 2.0), abs(this->width) / 2.0, abs(this->height) / 2.0, this->useDensity);
            break;
    }

    if (ent != NULL) {
        ent->setTranslation(this->underGroupNode->getRevertTranslation());
        ent->applyTranslation();
        ent->rebuild();

        this->underGroupNode->addNode(ent);
    }
}

void UserDrawHelp::drawPoints(GraphicPrinter & print, std::vector<Point> & ls, int flag) {
    if (ls.size() > 0) {

        Color col(30, 30, 30);

        if (flag == 1) {
            col = Color(200, 200, 200);
        }

        for (std::vector<Point>::iterator _it = ls.begin(); _it != ls.end(); ++_it) {

            Point * ptFrom = &(*_it);
            Point * ptTo = ptFrom->getSnapTo();

            if (ptTo != NULL) {
                print.drawLine(col, *ptFrom, *ptTo);
            }
        }

        if (flag == 0) {
            for (std::vector<Point>::iterator _it = ls.begin(); _it != ls.end(); ++_it) {
                (*_it).DrawVisualized(print, flag);
            }
        }

    } // if
}

void UserDrawHelp::draw(GraphicPrinter & print) {

    this->drawPoints(print, this->finalPoints, 1);
    this->drawPoints(print, this->points, 0);

}
