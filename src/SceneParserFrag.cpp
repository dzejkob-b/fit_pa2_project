#include "SceneParserFrag.h"
#include "SceneParser.h"
#include "DrawSquare.h"
#include "DrawCircle.h"
#include "DrawEllipse.h"
#include "DrawRectangle.h"
#include "DrawShortcut.h"
#include <iostream>
#include <stdio.h>

SceneParserFrag::SceneParserFrag(SceneParser * parent) {
    this->parent = parent;
}

SceneParserFrag::~SceneParserFrag() {
}

bool SceneParserFrag::haveIdentify() {
    return this->identify.size() > 0;
}

void SceneParserFrag::apIdentify(char ch) {
    this->identify += ch;
}

void SceneParserFrag::apParamFrag(char ch) {
    this->paramFrag += ch;
}

void SceneParserFrag::storeParamFrag() {
    if (this->paramFrag.size() > 0) {
        this->params.push_back(this->paramFrag);
    }

    this->paramFrag.clear();
}

void SceneParserFrag::apContent(char ch) {
    this->content += ch;
}

void SceneParserFrag::doClear() {
    this->identify.clear();
    this->paramFrag.clear();
    this->content.clear();
    this->params.clear();
}

double SceneParserFrag::popParamDouble() {
    if (this->params.size() == 0) {
        return 0;
    }

    double result;
    sscanf(this->params[0].c_str(), "%lf", &result);
    this->params.erase(this->params.begin());
    return result;
}

int SceneParserFrag::popParamInt() {
    if (this->params.size() == 0) {
        return 0;
    }

    int result;
    sscanf(this->params[0].c_str(), "%i", &result);
    this->params.erase(this->params.begin());
    return result;
}

std::string SceneParserFrag::popParamString() {
    if (this->params.size() == 0) {
        return "";
    }

    std::string result = this->params[0];
    this->params.erase(this->params.begin());
    return result;
}

Point SceneParserFrag::popPoint() {
    double pX = this->popParamDouble();
    double pY = this->popParamDouble();

    return Point(pX, pY);
}

Color SceneParserFrag::popColor() {
    Color result = Color(DrawFiller::lastUseColor);

    for (std::vector<std::string>::iterator _it = this->params.begin(); _it != this->params.end(); ++_it) {
        if (result.setFromFlag((*_it).c_str())) {
            break;
        }
    }

    return result;
}

int SceneParserFrag::popFillStyle() {
    int result = DrawFiller::lastUseStyle;

    for (std::vector<std::string>::iterator _it = this->params.begin(); _it != this->params.end(); ++_it) {
        int tmp = FillStyle::getFromFlag((*_it).c_str());

        if (tmp != -1) {
            result = tmp;
            break;
        }
    }

    return result;
}

bool SceneParserFrag::popClosed() {
    for (std::vector<std::string>::iterator _it = this->params.begin(); _it != this->params.end(); ++_it) {
        if ((*_it).compare("closed") == 0) {
            return true;
        }
    }

    return false;
}

/**
* process parsed command
*/
int SceneParserFrag::store(DrawNode * underNd) {

    DrawEntity * ent = NULL;

    if (this->identify.compare("group") == 0) {
        // group

        if (this->params.size() >= 1) {

            DrawNode * grpNd = this->parent->getArea()->getGroup(this->params[0].c_str());

            if (this->content.size() > 0) {
                // add elements under group

                if (grpNd == NULL) {
                    // create group if not existing - otherwise append content to existing group

                    grpNd = new DrawNode();
                    grpNd->setNodeKey(this->popParamString().c_str());

                    double trX = this->popParamDouble();
                    double trY = this->popParamDouble();

                    grpNd->getTranslate().setOffset(trX, trY);
                    grpNd->getTranslate().applyFixed();

                    underNd->addNode(grpNd);

                } else {
                    // add eventual translation

                    this->popParamString();

                    double trX = this->popParamDouble();
                    double trY = this->popParamDouble();

                    grpNd->getTranslate().setOffset(trX, trY);
                    grpNd->getTranslate().applyFixed();
                }

                this->parent->parse(this->content.c_str(), grpNd);
            }
        }

    } else if (this->identify.compare("shortcut") == 0) {
        // shortcut

        if (this->params.size() >= 2) {

            DrawNode * grpNd = this->parent->getArea()->getGroup(this->popParamString().c_str());

            if (grpNd != NULL) {

                DrawNode * shNd = new DrawShortcut(grpNd);

                shNd->setNodeKey(this->popParamString().c_str());

                double trX = this->popParamDouble();
                double trY = this->popParamDouble();

                shNd->getTranslate().setOffset(trX, trY);
                shNd->getTranslate().applyFixed();

                underNd->addNode(shNd);
            }
        }

    } else if (this->identify.compare("polyline") == 0) {
        // polyline creation logic

        int ptCount = 0;

        if (this->params.size() >= 1 && (ptCount = (unsigned int)this->popParamInt()) > 0 && this->params.size() >= (unsigned int)ptCount * 2 + 1) {
            ent = new DrawEntity(this->popClosed());

            for (int sf = 0; sf < ptCount; sf++) {
                ent->addPoint(this->popPoint());
            }
        }

    } else if (this->identify.compare("square") == 0) {
        // square creation logic

        if (this->params.size() >= 3) {
            ent = new DrawSquare(this->popPoint(), this->popParamDouble());
        }

    } else if (this->identify.compare("rectangle") == 0) {
        // rectangle creation logic

        if (this->params.size() >= 4) {
            ent = new DrawRectangle(this->popPoint(), this->popParamDouble(), this->popParamDouble());
        }

    } else if (this->identify.compare("circle") == 0) {
        // circle creation logic

        if (this->params.size() >= 4) {
            ent = new DrawCircle(this->popPoint(), this->popParamDouble(), this->popParamInt());
        }

    } else if (this->identify.compare("ellipse") == 0) {
        // ellipse creation logic

        if (this->params.size() >= 5) {
            ent = new DrawEllipse(this->popPoint(), this->popParamDouble(), this->popParamDouble(), this->popParamInt());
        }
    }

    if (ent != NULL) {
        ent->getFiller()->setColor(this->popColor());
        ent->getFiller()->setStyle(this->popFillStyle());
        ent->rebuild();

        underNd->addNode(ent);
    }

    this->doClear();

    return 0;
}
