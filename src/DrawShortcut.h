#ifndef DRAWSHORTCUT_H_INCLUDED
#define DRAWSHORTCUT_H_INCLUDED

#include <iostream>
#include "DrawNode.h"

/**
* entity in DrawArea which representing link to existing group
* the result is duplicated image of all graphic elements in linked group at shortcut position
*/
class DrawShortcut : public DrawNode {
    private:
        DrawNode * groupLink;

        void shDraw(GraphicPrinter & print, DrawNode * _fromNd);

    public:
        DrawShortcut(DrawNode * _groupLink);
        virtual ~DrawShortcut();

        virtual int getType() const;

        virtual std::string getComposedText() const;
        DrawNode * getGroupLink();

        virtual void draw(GraphicPrinter & print);
};

#endif // DRAWSHORTCUT_H_INCLUDED
