#ifndef USERDRAWHELP_H_INCLUDED
#define USERDRAWHELP_H_INCLUDED

#include <list>
#include <vector>
#include "GraphicPrinter.h"
#include "Point.h"
#include "DrawArea.h"

/**
* logic providing middle states of user drawing of polylines, rectangles, ellipses
*/
class UserDrawHelp {
    private:
        int state = 0;

        std::vector<Point> points;
        std::vector<Point> finalPoints;

        int useDensity = 20;

        double ptX;
        double ptY;
        double width;
        double height;

        DrawNode * underGroupNode;

    public:
        UserDrawHelp();
        ~UserDrawHelp();

        void setState(int _newState, DrawNode * _underGroupNode);
        void addDensity(int _toAdd);

        bool havePoint();
        void addPoint(Point _pt);
        void setLinePoint(Point _pt);
        void setRectanglePoint(Point _pt, bool _sqare = false);
        void updatePreview();
        void clearPoints(bool _keepLast = false);
        void resetClean();

        void stackFinalPoints();
        void pushToDrawArea(DrawArea * _area, bool _closed = true);

        void drawPoints(GraphicPrinter & print, std::vector<Point> & ls, int flag);
        void draw(GraphicPrinter & print);
};

#endif // USERDRAWHELP_H_INCLUDED
