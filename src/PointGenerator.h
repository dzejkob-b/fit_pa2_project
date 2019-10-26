#ifndef POINTGENERATOR_H_INCLUDED
#define POINTGENERATOR_H_INCLUDED

#include "Point.h"
#include <list>
#include <vector>

/**
* class responsible of generating sum of points in ellipse shape in specified density
*/
class PointGenerator {
    private:
        std::vector<Point> points;
        int density = 10;

    public:
        PointGenerator();
        ~PointGenerator();

        void setDensity(int _density);
        void createEllipse(Point center, double radiusX, double radiusY);
        void appendTo(std::vector<Point> & trg);
        void buildContinuousSnaps(std::vector<Point> & trg, bool isClosed, int startIdx = 0, int toIdx = -1);
};

#endif // POINTGENERATOR_H_INCLUDED
