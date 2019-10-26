#ifndef DRAWAREA_H_INCLUDED
#define DRAWAREA_H_INCLUDED

#include <vector>
#include "IBorder.h"
#include "DrawEntity.h"
#include "GraphicPrinter.h"

/**
* drawing area with tree structure of contained elements (groups, links and graphic entities)
*/
class DrawArea : public IBorder {
    private:
        DrawNode * rootNode;
        std::string scenePath;

        Point minLeftTop;
        Point maxRightBottom;

    public:
        DrawArea();
        ~DrawArea();

        DrawNode * getRootNode();
        DrawNode * getNodeByCoords(Point _pt);
        DrawNode * getGroup(const char * key);
        std::string & getScenePath();
        int getNodesCount(int _type = -1);

        void reset();

        int loadTextFromFile(const char * _path);
        int loadTextString(const char * _content);
        int saveText(const char * _scenePath = NULL);
        int saveBitmap(const char * _scenePath = NULL);
        int saveAuto(const char * _path);

        void updateBorders();

        Point i_getLeftTop();
        Point i_getRightTop();
        Point i_getLeftBottom();
        Point i_getRightBottom();

        void logic();
        void draw(GraphicPrinter & print);
};

#endif // DRAWAREA_H_INCLUDED
