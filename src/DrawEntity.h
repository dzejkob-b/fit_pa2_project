#ifndef DRAWENTITY_H_INCLUDED
#define DRAWENTITY_H_INCLUDED

#include "Point.h"
#include "DrawFiller.h"
#include "DrawNode.h"
#include "IBorder.h"
#include "GraphicPrinter.h"
#include <vector>

/**
* top level graphic entity consisting of sum of points, which may be contained in groups
*/
class DrawEntity : public DrawNode, public IBorder {
    protected:
        std::vector<Point> points;

        DrawFiller filler;

        Point minLeftTop;
        Point maxRightBottom;

        bool isClosed = false;

    public:
        DrawEntity(bool _isClosed);
        virtual ~DrawEntity();

        void addPoint(Point _pt);
        void addPointList(std::vector<Point> & _ls);

        virtual int getType() const;
        std::string getTypeReadable(bool _details) const;
        std::string getComposedText() const;

        DrawFiller * getFiller();
        bool isInside(Point & _pt);
        std::vector<Point> collidePoints(Point & ptRay, Point & ptDir);

        virtual DrawNode * getClone();

        Point i_getLeftTop();
        Point i_getRightTop();
        Point i_getLeftBottom();
        Point i_getRightBottom();

        virtual void applyTranslation();
        virtual void rebuild();

        void draw(GraphicPrinter & print);
        void drawFill(GraphicPrinter & print);

        friend std::ostream & operator << (std::ostream & os, const DrawEntity & _ref) {
            os << _ref.getTypeReadable(true);

            for (std::vector<Point>::const_iterator _it = _ref.points.begin(); _it != _ref.points.end(); ++_it) {
                os << ", " << *_it;
            }

            return os;
        }
};

#endif // DRAWENTITY_H_INCLUDED
