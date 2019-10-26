#ifndef COLOR_H_INCLUDED
#define COLOR_H_INCLUDED

#include "SDL2/SDL.h"
#include <string>
#include <map>

/**
* basic class for RGBA color representation mapped by string names
*/
class Color {
    private:
    public:
        static std::map<std::string, Color> colors;

        std::string flag;
        int r;
        int g;
        int b;
        int a;

        Color();
        Color(const Color & _ref);
        Color(int _r, int _g, int _b, int _a = 255);
        Color(int _r, int _g, int _b, int _a, const char * _flag);
        Color(const char * _flag);

        const std::string & getFlag() const;
        bool setFromFlag(const char * _flag);
        void setCol(int _r, int _g, int _b, int _a = 255);
};

#endif // COLOR_H_INCLUDED
