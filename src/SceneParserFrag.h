#ifndef SCENEPARSERFRAG_H_INCLUDED
#define SCENEPARSERFRAG_H_INCLUDED

#include "Color.h"
#include "Point.h"
#include "DrawNode.h"
#include <string>
#include <vector>
#include <ostream>

class SceneParser;

/**
* SceneParser item which representing one command with parameters
*/
class SceneParserFrag {
    private:
        SceneParser * parent;

        std::string identify;
        std::string paramFrag;
        std::vector<std::string> params;
        std::string content;

    public:
        SceneParserFrag(SceneParser * parent);
        ~SceneParserFrag();

        bool haveIdentify();

        void apIdentify(char ch);
        void apParamFrag(char ch);
        void storeParamFrag();
        void apContent(char ch);
        void doClear();

        double      popParamDouble();
        int         popParamInt();
        std::string popParamString();
        Point       popPoint();
        Color       popColor();
        bool        popClosed();
        int         popFillStyle();

        int store(DrawNode * underNd);

        friend std::ostream & operator << (std::ostream & os, const SceneParserFrag & _ref) {
            os << "Frag, identify: " << _ref.identify;

            for (std::vector<std::string>::const_iterator _it = _ref.params.begin(); _it != _ref.params.end(); ++_it) {
                os << ", P: " << (*_it);
            }

            os << ", content: " << _ref.content;

            return os;
        }
};

#endif // SCENEPARSERFRAG_H_INCLUDED
