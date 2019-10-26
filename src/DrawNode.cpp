#include "DrawNode.h"
#include "DrawShortcut.h"
#include <list>
#include <vector>
#include <iostream>
#include <algorithm>

DrawNode::DrawNode() {
    this->parentNode = NULL;

    this->parents.clear();
    this->parents.push_back(this);

    this->transTempLink = NULL;
}

DrawNode::~DrawNode() {
}

void DrawNode::setNodeKey(const char * _key) {
    this->nodeKey = std::string(_key);
}

/**
* set DrawNode key which is unique among all objects in tree structure
* on possible duplicated keys are added numbers
*/
void DrawNode::setNodeKeyUnique(const char * key) {
    int num = 1;
    bool found = false;

    std::stringstream testKey;

    do {
        found = false;

        testKey.str("");
        testKey << key;

        if (num > 1) {
            testKey << "_" << num;
        }

        std::list<DrawNode *> stk;

        stk.push_back(this->getMostParentNode());

        while (stk.size() > 0) {
            DrawNode * current = stk.back();
            stk.pop_back();

            if (current->nodeKey.compare(testKey.str()) == 0) {
                found = true;
                num++;
                break;

            } else if (current->haveChilds()) {
                for (auto _it = current->getChilds().begin(); _it != current->getChilds().end(); ++_it) {
                    stk.push_back(*_it);
                }
            }
        }
    } while (found);

    this->nodeKey = testKey.str();
}

std::string & DrawNode::getNodeKey() {
    return this->nodeKey;
}

int DrawNode::getType() const {
    return DrawNode::TYPE_GROUP;
}

DrawNode * DrawNode::getParentNode() {
    return this->parentNode;
}
DrawNode * DrawNode::getMostParentNode() {
    DrawNode * loop = this;

    while (loop->parentNode != NULL) {
        loop = loop->parentNode;
    }

    return loop;
}

Translate & DrawNode::getTranslate() {
    return this->trans;
}

bool DrawNode::haveChilds() {
    return this->childs.size() > 0;
}

bool DrawNode::haveGrpChilds() {
    for (auto _it = this->childs.begin(); _it != this->childs.end(); ++_it) {
        if ((*_it)->getType() == DrawNode::TYPE_GROUP || (*_it)->getType() == DrawNode::TYPE_SHORTCUT) {
            return true;
        }
    }

    return false;
}

const std::vector<DrawNode *> & DrawNode::getChilds() {
    return this->childs;
}

bool DrawNode::isInside(Point & _pt) {
    return false;
}

/**
* @return returns more readable text representation of node type
*/
std::string DrawNode::getTypeReadable(bool _details) const {
    std::stringstream str;

    switch (this->getType()) {
        case DrawNode::TYPE_GROUP :
            str << "Group " << this->nodeKey; break;

        case DrawNode::TYPE_SHORTCUT :
            str << "Shortcut " << this->nodeKey << " to " << ((DrawShortcut *)this)->getGroupLink()->getNodeKey(); break;

        case DrawNode::TYPE_ENTITY :
            str << "Polyline"; break;

        case DrawNode::TYPE_SQUARE :
            str << "Square"; break;

        case DrawNode::TYPE_RECTANGLE :
            str << "Rectangle"; break;

        case DrawNode::TYPE_CIRCLE :
            str << "Circle"; break;

        case DrawNode::TYPE_ELLIPSE :
            str << "Ellipse"; break;
    }

    return str.str();
}

/**
* @return returns readable text path representation
*/
std::string DrawNode::getPathReadable() const {
    std::list<std::string> pts;

    DrawNode * loop = this->parentNode;

    while (loop != NULL) {
        pts.push_front(loop->getNodeKey());
        loop = loop->parentNode;
    }

    std::string result;

    for (std::list<std::string>::iterator _it = pts.begin(); _it != pts.end(); ++_it) {
        if (result.size() > 0) {
            result.append(" | ");
        }

        result.append(*_it);
    }

    return result;
}

std::string DrawNode::getComposedText() const {
    std::stringstream str;

    str << "group(" << this->nodeKey << ", " << this->trans.getFixedOffsetX() << ", " << this->trans.getFixedOffsetY() << ")";

    return str.str();
}

DrawNode * DrawNode::getClone() {
    DrawNode * nRef = new DrawNode();

    nRef->setNodeKey(this->nodeKey.c_str());
    nRef->trans = Translate(this->trans);

    return nRef;
}

/**
* add DrawNode under this node (valid only for DrawNode::TYPE_GROUP)
*/
void DrawNode::addNode(DrawNode * _ref) {
    if (this->getType() == DrawNode::TYPE_GROUP) {

        this->childs.push_back(_ref);

        _ref->parentNode = this;
        _ref->parents.clear();

        DrawNode * loop = _ref;

        while (loop != NULL) {
            _ref->parents.push_back(loop);
            loop = loop->parentNode;
        }
    }
}

/**
* add duplicated specified node and all its children under this node (valid only for DrawNode::TYPE_GROUP)
* @return returns top level duplicated DrawNode
*/
DrawNode * DrawNode::addNodeDuplicate(DrawNode * _ref) {

    DrawNode * firstDupNode = NULL;

    if (this->getType() == DrawNode::TYPE_GROUP) {

        typedef struct {

            DrawNode * currentNode;
            DrawNode * parentDupNode;

        } treeLoop;

        std::list<treeLoop> stk;

        treeLoop c;

        c.currentNode = _ref;
        c.parentDupNode = this;

        stk.push_back(c);

        while (stk.size() > 0) {
            c = stk.back();
            stk.pop_back();

            DrawNode * dupNode = c.currentNode->getClone();

            if (firstDupNode == NULL) {
                firstDupNode = dupNode;
            }

            c.parentDupNode->addNode(dupNode);

            dupNode->rebuild();

            if (dupNode->getType() == DrawNode::TYPE_GROUP) {
                dupNode->setNodeKeyUnique(c.currentNode->getNodeKey().c_str());
            }

            if (c.currentNode->haveChilds()) {
                for (auto _it = c.currentNode->getChilds().rbegin(); _it != c.currentNode->getChilds().rend(); ++_it) {

                    treeLoop sc;

                    sc.currentNode = (*_it);
                    sc.parentDupNode = dupNode;

                    stk.push_back(sc);
                }
            }
        } // while

    }

    return firstDupNode;
}

void DrawNode::delNode(DrawNode * _ref) {

    if (_ref->getType() == DrawNode::TYPE_GROUP) {
        // remove all possible shortcuts to this group

        this->getMostParentNode()->delShortcutsToNode(_ref);

        _ref->delAllChilds();
    }

    std::vector<DrawNode *>::iterator _it = std::find(this->childs.begin(), this->childs.end(), _ref);

    if (_it != this->childs.end()) {
        this->childs.erase(_it);

        delete _ref;
        _ref = NULL;
    }
}

void DrawNode::delAllChilds() {
    if (this->haveChilds()) {
        while (this->childs.size() > 0) {
            this->delNode(this->childs[0]);
        }
    }
}

/**
* delete all DrawShortcuts which pointing to specified group node
* @param _ref pointer to entity of type DrawNode::TYPE_GROUP
*/
void DrawNode::delShortcutsToNode(DrawNode * _ref) {

    std::list<DrawNode *> stk;

    stk.push_back(this);

    while (stk.size() > 0) {
        DrawNode * current = stk.back();
        stk.pop_back();

        if (current->getType() == DrawNode::TYPE_SHORTCUT && ((DrawShortcut *)current)->getGroupLink() == _ref) {
            current->getParentNode()->delNode(current);

        } else if (current->haveChilds()) {
            for (auto _it = current->childs.rbegin(); _it != current->childs.rend(); ++_it) {
                stk.push_back(*_it);
            }
        }
    }
}

DrawNode * DrawNode::findNodeByKey(const char * str) {
    return this->findNode([](DrawNode * _loop, const void * _cmp) { return _loop->nodeKey.compare((const char *)_cmp) == 0; }, str);
}

/**
* abstract child node finding logic
*/
DrawNode * DrawNode::findNode(bool (*_fn)(DrawNode * _loop, const void * _cmp), const void * _toCmp) {

    std::list<DrawNode *> stk;

    stk.push_back(this);

    while (stk.size() > 0) {
        DrawNode * current = stk.back();
        stk.pop_back();

        if (_fn(current, _toCmp)) {
            return current;
        }

        for (std::vector<DrawNode *>::iterator _it = current->childs.begin(); _it != current->childs.end(); ++_it) {
            stk.push_back(*_it);
        }
    }

    return NULL;
}

/**
* apply all translations to point from most parent node to current node
*/
void DrawNode::applyTranslationChain(Point & _pt) {
    if (this->transTempLink == NULL) {
        // standard translation (default draw)

        for (std::vector<DrawNode *>::reverse_iterator _it = this->parents.rbegin(); _it != this->parents.rend(); ++_it) {
            (*_it)->trans.translate(_pt);
        }

    } else {
        // translation from shortcut

        for (std::vector<DrawNode *>::reverse_iterator _it = this->transTempLink->parents.rbegin(); _it != this->transTempLink->parents.rend(); ++_it) {
            (*_it)->trans.translate(_pt);
        }
    }
}

Translate DrawNode::getRevertTranslation() {
    Translate tr;

    for (std::vector<DrawNode *>::iterator _it = this->parents.begin(); _it != this->parents.end(); ++_it) {
        tr.translateRevert((*_it)->trans);
    }

    return tr;
}

/**
* set temporary link to DrawShortcut used for shortcut rendering
*/
void DrawNode::setTransTempLink(DrawNode * _transTempLink) {
    this->transTempLink = _transTempLink;
}

void DrawNode::setTranslation(Translate tr) {
    this->trans = tr;
}

void DrawNode::applyTranslation() {
    this->trans.applyFixed();
}

/**
* process rebuild of cache or graphical structure or any other possible stuff of inherited object
*/
void DrawNode::rebuild() {
    for (std::vector<DrawNode *>::iterator _it = this->childs.begin(); _it != this->childs.end(); ++_it) {
        (*_it)->rebuild();
    }
}

void DrawNode::draw(GraphicPrinter & print) {
}

void DrawNode::drawAll(GraphicPrinter & print) {

    this->draw(print);

    for (std::vector<DrawNode *>::iterator _it = this->childs.begin(); _it != this->childs.end(); ++_it) {
        (*_it)->drawAll(print);
    }
}
