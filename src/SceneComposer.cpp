#include "SceneComposer.h"

SceneComposer::SceneComposer(DrawArea * _area) {
    this->area = _area;
}

SceneComposer::~SceneComposer() {
}

DrawArea * SceneComposer::getArea() {
    return this->area;
}

int SceneComposer::saveIdent(int _ident) {
    int result = 0;

    for (int sf = 0; sf < _ident; sf++) {
        this->fHand << "    ";
    }

    return result;
}

/**
* save logic of each tree node
*/
int SceneComposer::saveLoop(DrawNode * _nd, int _ident) {
    int result = 0;

    for (std::vector<DrawNode *>::const_iterator _it = _nd->getChilds().begin(); _it != _nd->getChilds().end(); ++_it) {

        if ((result = this->saveIdent(_ident)) != 0) {
            break;
        }

        DrawNode * current = *_it;

        if (current->getType() == DrawNode::TYPE_GROUP) {
            this->fHand << current->getComposedText() << " {\n";

            if (this->fHand.fail()) {
                result = -1;
                break;

            } else if (current->haveChilds()) {
                this->saveLoop(current, _ident + 1);
            }

            if ((result = this->saveIdent(_ident)) != 0) {
                break;
            }

            this->fHand << "}\n";

        } else {
            this->fHand << current->getComposedText() << "\n";
        }

        if (this->fHand.fail()) {
            result = -1;
            break;
        }
    }

    return result;
}

/**
* save DrawArea objects in tree to readable text file
* @return returns zero on success
*/
int SceneComposer::save(const char * fileName) {
    int result = 0;

    this->fHand.open(fileName, std::ios::out);

    if (!this->fHand.fail() && this->fHand.is_open()) {

        DrawNode * rootNode = this->area->getRootNode();

        this->fHand << rootNode->getComposedText() << " {\n";

        if (this->fHand.fail()) {
            result = -1;

        } else if ((result = this->saveLoop(rootNode, 1)) == 0) {

            this->fHand << "}\n";

            if (this->fHand.fail()) {
                result = -1;
            }
        }

    } else {
        // write error
        result = -1;
    }

    fHand.close();

    return result;
}
