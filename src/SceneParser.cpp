#include "SceneParser.h"
#include "SceneParserFrag.h"
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>

SceneParser::SceneParser(DrawArea * _area) {
    this->area = _area;
}

SceneParser::~SceneParser() {
}

DrawArea * SceneParser::getArea() {
    return this->area;
}

/**
* parsing logic and preparing SceneParserFrag fragments
*/
int SceneParser::parse(const char * _str, DrawNode * _underNd) {

    int result = 0;
    int idx = 0;
    int len = strlen(_str);
    int bracket = 0;

    short flag = 0;
    bool spcFlag = false;

    SceneParserFrag frag(this);

    while (idx < len) {
        char ch = _str[idx];

        if (flag == 0) {
            switch (ch) {
                case ' ' :
                case '\n' :
                case '\r' :
                case '\t' :
                    spcFlag = true;
                    break;

                case '{' :
                    if (frag.haveIdentify()) {
                        flag = 2;
                        bracket = 1;
                    }
                    break;

                case '(' :
                    if (frag.haveIdentify()) {
                        flag = 1;
                        bracket = 1;
                    }
                    break;

                default :
                    if (spcFlag && frag.haveIdentify()) {
                        frag.store(_underNd);
                    }

                    frag.apIdentify(ch);
                    spcFlag = false;
                    break;
            }

        } else if (flag == 1) {
            // param parsing

            switch (ch) {
                case '(' :
                    bracket = 1;
                    frag.apParamFrag(ch);
                    break;

                case ')' :
                    bracket--;

                    if (bracket == 0) {
                        frag.storeParamFrag();
                        flag = 0;

                    } else {
                        frag.apParamFrag(ch);
                    }
                    break;

                case ' ' :
                case '\n' :
                case '\r' :
                case '\t' :
                    break;

                case ',' :
                    frag.storeParamFrag();
                    break;

                default :
                    frag.apParamFrag(ch);
                    break;
            }

        } else if (flag == 2) {
            // sub content

            switch (ch) {
                case '{' :
                    bracket++;
                    frag.apContent(ch);
                    break;

                case '}' :
                    bracket--;

                    if (bracket == 0) {
                        frag.store(_underNd);
                        flag = 0;

                    } else {
                        frag.apContent(ch);
                    }
                    break;

                default :
                    frag.apContent(ch);
                    break;
            }
        }

        idx++;
    }

    if (frag.haveIdentify()) {
        frag.store(_underNd);
    }

    return result;
}

/**
* restore DrawArea objects from readable text file
* @return returns zero on success
*/
int SceneParser::loadFile(const char * _fileName) {
    int result = 0;

    std::ifstream fHand;

    fHand.open(_fileName);

    if (!fHand.fail() && fHand.is_open()) {
        std::string content;
        char buffer[33];

        while (!fHand.fail() && !fHand.eof()) {
            fHand.read(buffer, 32);

            if (fHand.eof() || !fHand.fail()) {
                buffer[fHand.gcount()] = '\0';
                content.append(buffer);

            } else {
                result = -1;
            }
        }

        if (result == 0) {
            result = this->parse(content.c_str(), this->area->getRootNode());
        }
    }

    fHand.close();

    return result;
}

/**
* restore DrawArea objects from readable text contained in string
* @return returns zero on success
*/
int SceneParser::loadString(const char * _content) {
    return this->parse(_content, this->area->getRootNode());
}
