#ifndef DRAWNODE_H_INCLUDED
#define DRAWNODE_H_INCLUDED

#include "Point.h"
#include "Translate.h"
#include "GraphicPrinter.h"
#include <vector>
#include <string>

/**
* basic entity contained in DrawArea which representing group of objects
* and implementing common logic in tree structure
*/
class DrawNode {
    protected:
        DrawNode * parentNode;

        std::vector<DrawNode *> childs;
        std::vector<DrawNode *> parents;
        std::string nodeKey;

        DrawNode * transTempLink;
        Translate trans;

    public:
        const static int TYPE_GROUP = 0;
        const static int TYPE_ENTITY = 1;
        const static int TYPE_SQUARE = 2;
        const static int TYPE_RECTANGLE = 3;
        const static int TYPE_CIRCLE = 4;
        const static int TYPE_ELLIPSE = 5;
        const static int TYPE_SHORTCUT = 6;

        DrawNode();
        virtual ~DrawNode();

        void setNodeKey(const char * _key);
        void setNodeKeyUnique(const char * key);
        std::string & getNodeKey();

        virtual int getType() const;
        DrawNode * getParentNode();
        DrawNode * getMostParentNode();
        Translate & getTranslate();
        bool haveChilds();
        bool haveGrpChilds();
        const std::vector<DrawNode *> & getChilds();

        virtual bool isInside(Point & _pt);

        virtual std::string getTypeReadable(bool _details) const;
        std::string getPathReadable() const;
        virtual std::string getComposedText() const;

        virtual DrawNode * getClone();

        void addNode(DrawNode * _ref);
        DrawNode * addNodeDuplicate(DrawNode * _ref);
        void delNode(DrawNode * _ref);
        void delShortcutsToNode(DrawNode * _ref);
        void delAllChilds();

        DrawNode * findNodeByKey(const char * str);
        DrawNode * findNode(bool (*_fn)(DrawNode * _loop, const void * _cmp), const void * _toCmp);

        void applyTranslationChain(Point & _pt);
        Translate getRevertTranslation();

        void setTransTempLink(DrawNode * _transTempLink);
        void setTranslation(Translate tr);
        virtual void applyTranslation();
        virtual void rebuild();

        virtual void draw(GraphicPrinter & print);
        void drawAll(GraphicPrinter & print);
};

#endif // DRAWNODE_H_INCLUDED
