#ifndef SCENECOMPOSER_H_INCLUDED
#define SCENECOMPOSER_H_INCLUDED

#include <iostream>
#include <fstream>
#include "DrawArea.h"

/**
* class responsible of composing logical structure of tree objects of DrawArea to text readable file
*/
class SceneComposer {
    private:
        DrawArea * area;

        std::ofstream fHand;

        int saveIdent(int _ident);
        int saveLoop(DrawNode * _nd, int _ident = 0);

    public:
        SceneComposer(DrawArea * _area);
        ~SceneComposer();

        DrawArea * getArea();

        int save(const char * fileName);
};

#endif // SCENECOMPOSER_H_INCLUDED
