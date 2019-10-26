#ifndef POINT_H_INCLUDED
#define POINT_H_INCLUDED

#include <vector>
#include "Point.h"
#include "IRenderer.h"

class GraphicPrinter;

/**
* base class representing Point and providing features like distance or line collision
*/
class Point {
    private:
        double x;
        double y;
        double meta;

        Point * snapTo = NULL;

    public:
        Point();
        Point(const Point & _rf);
        Point(double _x, double _y);
        Point(int _x, int _y);
        ~Point();

        void DrawVisualized(GraphicPrinter & printer, int flag);

        double getX() const;
        double getY() const;

        int getIntX() const;
        int getIntY() const;

        void setVals(double _x, double _y);
        void setVals(const Point & _rf);
        void addVals(double _x, double _y);
        void addRadius(double _angle, double _radius);
        void setX(double _x);
        void setY(double _y);

        void setMeta(double _meta);
        double getMeta() const;

        double distTo(Point & _rf);
        double distTo(double _x, double _y);

        void setSnapTo(Point * _snapTo);
        Point * getSnapTo() const;

        bool collideLineRay(const Point & _ptTo, const Point & _colFrom, const Point & _colTo);
        bool collideLineAppendSorted(const Point & _ptTo, const Point & _colFrom, const Point & _colTo, std::vector<Point> * _appendTo);

        friend std::ostream & operator << (std::ostream & os, const Point & _ref) {
            os << "P[" << _ref.x << ", " << _ref.y << "]";

            return os;
        }

        friend std::ostream & operator << (std::ostream & os, const std::vector<Point> & _refs) {
            for (std::vector<Point>::const_iterator _it = _refs.begin(); _it != _refs.end(); ++_it) {
                os << ", " << *_it;
            }

            return os;
        }
};

#endif // POINT_H_INCLUDED
