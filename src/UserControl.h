#ifndef USERCONTROL_H_INCLUDED
#define USERCONTROL_H_INCLUDED

#include "GraphicPrinter.h"
#include "MenuCollector.h"
#include "UserDrawHelp.h"
#include "DrawArea.h"

/**
* main logic of graphic editor in edit mode
* providing user interface menus for various editing scenarios
*/
class UserControl {
    private:
        MenuCollector   * menu = NULL;
        Menu            * rootMenu = NULL;
        Menu            * createStateMenu = NULL;
        Menu            * creatingMenu = NULL;
        Menu            * modifyMenu = NULL;
        DrawArea        * area = NULL;
        UserDrawHelp      drawHelp;
        DrawNode        * manNode = NULL;

        int state = UserControl::STATE_NONE;

        Point moveOffset;
        int moveFlag = 0;

        void createRootMenu();
        void createCreatingMenu();
        void createModifyMenu(bool _keepCoord = false);
        void stateTransfer(int _newState);

    public:
        static const int STATE_NONE = 0;
        static const int STATE_MODIFY = 1;
        static const int STATE_MOVE = 2;
        static const int STATE_POLYLINE = 10;
        static const int STATE_SQUARE = 11;
        static const int STATE_RECTANGLE = 12;
        static const int STATE_CIRCLE = 13;
        static const int STATE_ELLIPSE = 14;

        UserControl(MenuCollector * _menu, DrawArea * _area);
        ~UserControl();

        void logic();
        void draw(GraphicPrinter & print);
};

#endif // USERCONTROL_H_INCLUDED
