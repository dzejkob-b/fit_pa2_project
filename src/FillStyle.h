#ifndef FILLSTYLE_H_INCLUDED
#define FILLSTYLE_H_INCLUDED

/**
* basic class for fill styles and string conversion
*/
class FillStyle {
    private:
    public:
        static const int STYLE_NONE = 0;
        static const int STYLE_SOLID = 1;
        static const int STYLE_LINED45 = 2;
        static const int STYLE_LINED45_REV = 3;
        static const int STYLE_LINECROSS = 4;

        static int getFromFlag(const char * _flag);
        static const char * getFlagFromStyle(int _style);
};

#endif // FILLSTYLE_H_INCLUDED
