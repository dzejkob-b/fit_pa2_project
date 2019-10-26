#ifndef GRAPHICPRINTER_H_INCLUDED
#define GRAPHICPRINTER_H_INCLUDED

#include "Point.h"
#include "Color.h"
#include "Rect.h"

/**
* abstract class for graphic output
*/
class GraphicPrinter {
    protected:
        Point tmpFrom;
        Point tmpTo;
        Point tmpPt;

    public:
        GraphicPrinter();
        virtual ~GraphicPrinter();

        virtual void drawPixel(Color & col, Point & pt);
        virtual void drawLine(Color & col, Point & ptFrom, Point & ptTo);
        virtual void drawLine(Color & col, double fromX, double fromY, double toX, double toY);
        virtual void drawRectangle(Color & col, Rect & re);
        virtual void drawFilledRectangle(Color & col, Rect & re);
};

#endif // GRAPHICPRINTER_H_INCLUDED
