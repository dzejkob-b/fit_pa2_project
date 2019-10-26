#include "DrawArea.h"
#include "Services.h"
#include "SceneParser.h"
#include "SceneComposer.h"
#include "GraphicPrinterBMP.h"
#include <algorithm>
#include <iostream>
#include <list>

DrawArea::DrawArea() {
    this->rootNode = new DrawNode();
    this->rootNode->setNodeKey("scene");

    this->scenePath = "default";
}

DrawArea::~DrawArea() {
    this->rootNode->delAllChilds();
    delete this->rootNode;
    this->rootNode = NULL;
}

DrawNode * DrawArea::getRootNode() {
    return this->rootNode;
}

/**
* @return find first graphical node, which have specified Point inside (valid for all types of shapes)
*/
DrawNode * DrawArea::getNodeByCoords(Point _pt) {

    return this->rootNode->findNode([](DrawNode * _loop, const void * _cmp) {

        return _loop->isInside(*((Point *)_cmp));

    }, &_pt);
}

/**
* @return find DrawNode of type DrawNode::TYPE_GROUP and specified key
*/
DrawNode * DrawArea::getGroup(const char * key) {
    return this->rootNode->findNodeByKey(key);
}

std::string & DrawArea::getScenePath() {
    return this->scenePath;
}

/**
* @param _type filter param by given type
* @return count of all nodes in area
*/
int DrawArea::getNodesCount(int _type) {
    int result = 0;

    std::list<DrawNode *> stk;

    stk.push_back(this->rootNode);

    while (stk.size() > 0) {
        DrawNode * current = stk.back();
        stk.pop_back();

        if (_type == -1 || current->getType() == _type) {
            result++;
        }

        if (current->haveChilds()) {
            for (auto _it = current->getChilds().begin(); _it != current->getChilds().end(); ++_it) {
                stk.push_back(*_it);
            }
        }
    }

    return result;
}

/**
* resets scene - delete all nodes and subnodes and create new root "scene" group node
*/
void DrawArea::reset() {
    this->rootNode->delAllChilds();
    delete this->rootNode;

    this->rootNode = new DrawNode();
    this->rootNode->setNodeKey("scene");
}

int DrawArea::loadTextFromFile(const char * _path) {

    std::string path(_path);

    unsigned int idx = path.find_last_of(".");

    if (idx != std::string::npos) {
        path = path.substr(0, idx);
    }

    // path without suffix

    this->scenePath = path;
    this->reset();

    SceneParser parse(this);

    return parse.loadFile(std::string(this->scenePath).append(".txt").c_str());
}

int DrawArea::loadTextString(const char * _content) {

    SceneParser parse(this);

    return parse.loadString(_content);
}

int DrawArea::saveText(const char * _scenePath) {

    std::string useScenePath;

    if (_scenePath != NULL) {
        useScenePath = _scenePath;
    } else {
        useScenePath = this->scenePath;
    }

    if (useScenePath.length() == 0) {
        // no scene name defined
        return -1;
    }

    SceneComposer compose(this);

    return compose.save(useScenePath.append(".txt").c_str());
}

int DrawArea::saveBitmap(const char * _scenePath) {
    int result = 0;

    std::string useScenePath;

    if (_scenePath != NULL) {
        useScenePath = std::string(_scenePath);
    } else {
        useScenePath = std::string(this->scenePath);
    }

    if (useScenePath.length() == 0) {
        // no scene name defined
        return -1;
    }

    this->updateBorders();

    std::ofstream fHand;

    fHand.open(useScenePath.append(".bmp"), std::ios::out | std::ios::binary);

    if (!fHand.fail() && fHand.is_open()) {

        GraphicPrinterBMP print;

        if ((result = print.initialize(this)) == 0) {

            this->draw(print);

            if ((result = print.output(fHand)) == 0) {
                // successfully saved ...

            }
        }

    } else {
        // write error
        result = -1;
    }

    fHand.close();

    return result;
}

/**
* automatically determine format of given path by extension (if extension is blank, assuming .txt)
* @return returns non zero if save failed
*/
int DrawArea::saveAuto(const char * _path) {

    std::string path(_path);
    std::string ext;

    size_t idx = path.find_last_of(".");

    if (idx != std::string::npos) {
        ext = path.substr(idx + 1);
        path = path.substr(0, idx);
    }

    if (ext.size() == 0 || ext.compare("txt") == 0) {
        // save text
        return this->saveText(path.c_str());

    } else if (ext.compare("bmp") == 0) {
        // save bitmap
        return this->saveBitmap(path.c_str());

    } else {
        return -8;
    }
}

void DrawArea::updateBorders() {

    std::list<DrawNode *> stk;

    stk.push_back(this->getRootNode());

    bool firstSet = false;

    while (stk.size() > 0) {
        DrawNode * current = stk.back();
        stk.pop_back();

        if (
            current->getType() == DrawNode::TYPE_ENTITY ||
            current->getType() == DrawNode::TYPE_RECTANGLE ||
            current->getType() == DrawNode::TYPE_SQUARE ||
            current->getType() == DrawNode::TYPE_ELLIPSE ||
            current->getType() == DrawNode::TYPE_CIRCLE
        ) {

            Point leftTop = ((DrawEntity *)current)->i_getLeftTop();
            Point rightBottom = ((DrawEntity *)current)->i_getRightBottom();

            if (!firstSet) {
                firstSet = true;

                this->minLeftTop.setVals(leftTop);
                this->maxRightBottom.setVals(rightBottom);

            } else {
                if (leftTop.getX() < this->minLeftTop.getX()) this->minLeftTop.setX(leftTop.getX());
                if (leftTop.getY() < this->minLeftTop.getY()) this->minLeftTop.setY(leftTop.getY());

                if (rightBottom.getX() > this->maxRightBottom.getX()) this->maxRightBottom.setX(rightBottom.getX());
                if (rightBottom.getY() > this->maxRightBottom.getY()) this->maxRightBottom.setY(rightBottom.getY());
            }
        }

        if (current->haveChilds()) {
            for (auto _it = current->getChilds().begin(); _it != current->getChilds().end(); ++_it) {
                stk.push_back(*_it);
            }
        }
    }
}

Point DrawArea::i_getLeftTop() {
    return Point(this->minLeftTop);
}

Point DrawArea::i_getRightTop() {
    return Point(this->maxRightBottom.getX(), this->minLeftTop.getY());
}

Point DrawArea::i_getLeftBottom() {
    return Point(this->minLeftTop.getX(), this->maxRightBottom.getY());
}

Point DrawArea::i_getRightBottom() {
    return Point(this->maxRightBottom);
}

void DrawArea::logic() {

}

void DrawArea::draw(GraphicPrinter & print) {

    this->rootNode->drawAll(print);

}
