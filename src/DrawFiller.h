#ifndef DRAWFILLER_H_INCLUDED
#define DRAWFILLER_H_INCLUDED

#include <vector>
#include "GraphicPrinter.h"
#include "IBorder.h"
#include "Point.h"
#include "Color.h"
#include "FillStyle.h"

class DrawEntity;

/**
* class responsible of filling logic - may fill shape defined by sum of points
* supporting various fill styles and colors
*/
class DrawFiller {
    private:
        DrawEntity * entity;

        Point rayFrom;
        Point rayDir;

        int useStyle = FillStyle::STYLE_SOLID;
        Color useColor = Color(120, 120, 120);

        void (*callReset)(DrawFiller * _self) = NULL;
        void (*callStep)(DrawFiller * _self) = NULL;

        void rayStep();
        void fillLogic(GraphicPrinter & print);

    public:
        static int lastUseStyle;
        static Color lastUseColor;

        DrawFiller(DrawEntity * _entity);
        ~DrawFiller();

        const int getStyle() const;
        const Color & getColor() const;

        void setStyle(int _useStyle);
        void setColor(Color _useColor);
        void resetRay();
        void drawFill(GraphicPrinter & print);
};

#endif // DRAWFILLER_H_INCLUDED
