#include "FillStyle.h"
#include <string.h>

int FillStyle::getFromFlag(const char * _flag) {
    int result = -1;

    if (strcmp(_flag, "fill_none") == 0) {
        result = FillStyle::STYLE_NONE;

    } else if (strcmp(_flag, "fill_solid") == 0) {
        result = FillStyle::STYLE_SOLID;

    } else if (strcmp(_flag, "fill_line45") == 0) {
        result = FillStyle::STYLE_LINED45;

    } else if (strcmp(_flag, "fill_line45_rev") == 0) {
        result = FillStyle::STYLE_LINED45_REV;

    } else if (strcmp(_flag, "fill_linecross") == 0) {
        result = FillStyle::STYLE_LINECROSS;
    }

    return result;
}

const char * FillStyle::getFlagFromStyle(int _style) {
    switch (_style) {
        case FillStyle::STYLE_SOLID :
            return "fill_solid";

        case FillStyle::STYLE_LINED45 :
            return "fill_line45";

        case FillStyle::STYLE_LINED45_REV :
            return "fill_line45_rev";

        case FillStyle::STYLE_LINECROSS :
            return "fill_linecross";

        default :
            return "fill_none";
    }
}
