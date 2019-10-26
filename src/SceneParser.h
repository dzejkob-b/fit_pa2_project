#ifndef SCENEPARSER_H_INCLUDED
#define SCENEPARSER_H_INCLUDED

#include "DrawArea.h"
#include "DrawNode.h"

/**
* class responsible of parsing text file and reading logical tree object structure and constructing DrawArea
*/
class SceneParser {
    private:
        DrawArea * area;

    public:
        SceneParser(DrawArea * _area);
        ~SceneParser();

        DrawArea * getArea();

        int parse(const char * _str, DrawNode * _underNd);
        int loadFile(const char * _fileName);
        int loadString(const char * _content);
};

#endif // SCENEPARSER_H_INCLUDED
