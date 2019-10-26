#include "Color.h"
#include <string.h>

std::map<std::string, Color> Color::colors = {
    std::make_pair("gray", Color(120, 120, 120, 255, "gray")),
    std::make_pair("darkgray", Color(60, 60, 60, 255, "darkgray")),
    std::make_pair("white", Color(255, 255, 255, 255, "white")),
    std::make_pair("red", Color(209, 12, 12, 255, "red")),
    std::make_pair("blue", Color(26, 12, 214, 255, "blue")),
    std::make_pair("green", Color(15, 158, 46, 255, "green")),
    std::make_pair("yellow", Color(233, 237, 14, 255, "yellow")),
    std::make_pair("brown", Color(206, 143, 43, 255, "brown"))
};

Color::Color() {
    this->r = 0;
    this->g = 0;
    this->b = 0;
    this->a = 255;
    this->flag = "black";
}

Color::Color(const Color & _ref) {
    this->r = _ref.r;
    this->g = _ref.g;
    this->b = _ref.b;
    this->a = _ref.a;
    this->flag = _ref.flag;
}

Color::Color(int _r, int _g, int _b, int _a) {
    this->r = _r;
    this->g = _g;
    this->b = _b;
    this->a = _a;
    this->flag = "unknown";
}

Color::Color(int _r, int _g, int _b, int _a, const char * _flag) {
    this->r = _r;
    this->g = _g;
    this->b = _b;
    this->a = _a;
    this->flag = _flag;
}

Color::Color(const char * _flag) {
    this->r = 255;
    this->g = 255;
    this->b = 255;
    this->a = 255;
    this->flag = "white";

    this->setFromFlag(_flag);
}

/**
* @return if color was initialized from string constant, the corresponding key is returned
*/
const std::string & Color::getFlag() const {
    return this->flag;
}

/**
* @param flag key from string intexed constants
* @return returns true if string constant is valid
*/
bool Color::setFromFlag(const char * _flag) {

    auto _it = Color::colors.find(_flag);

    if (_it == Color::colors.end()) {
        return false;

    } else {
        this->r = (*_it).second.r;
        this->g = (*_it).second.g;
        this->b = (*_it).second.b;
        this->a = (*_it).second.a;
        this->flag = (*_it).second.flag;

        return true;
    }
}

void Color::setCol(int _r, int _g, int _b, int _a) {
    this->r = _r;
    this->g = _g;
    this->b = _b;
    this->a = _a;
}
