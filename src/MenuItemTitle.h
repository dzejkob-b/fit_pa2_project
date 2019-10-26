#ifndef MENUITEMTITLE_H_INCLUDED
#define MENUITEMTITLE_H_INCLUDED

#include "MenuItem.h"

/**
* specific menu title item inherited from MenuItem which is not clickable, but is higlighted
*/
class MenuItemTitle : public MenuItem {
    private:
    public:
        MenuItemTitle(Menu * _parent, const char * _key, const char * _caption);
        ~MenuItemTitle();

        bool isHoverable();
        bool isClickable();

        void draw(GraphicPrinter & print, int & posX, int & posY, int maxWidth);
};

#endif // MENUITEMTITLE_H_INCLUDED
