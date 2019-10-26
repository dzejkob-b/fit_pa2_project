#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <string>
#include <vector>

#include "GraphicPrinter.h"
#include "MenuItem.h"
#include "MenuItemTitle.h"

class MenuCollector;

/**
* class representing graphical context menu.
* possible of adding clickable choices with custom callbacks or simple title items
*/
class Menu {
    private:
        MenuCollector * parent;

        int coordX = 0;
        int coordY = 0;

        int maxWidth = 0;
        int maxHeight = 0;

        std::vector<MenuItem *> items;

        MenuItem * itemLast = NULL;
        MenuItem * itemHover = NULL;
        MenuItem * itemDown = NULL;

        void (* destroyCallback)(Menu * _self, void * _data) = NULL;
        void * destroyData = NULL;

        void dropChildMenus();

    public:
        void * custData = NULL;

        Menu(MenuCollector * _parent);
        ~Menu();

        MenuCollector * getParent();
        int getCoordX() const;
        int getCoordY() const;
        int getItemState(MenuItem * _item);
        void getItemCoords(MenuItem * _findItem, int & cX, int & cY);
        bool anyHover();

        Menu & setCoords(int _coordX, int _coordY);
        Menu & setParentItem(MenuItem * _parentItem);
        Menu & addItem(const char * _key, const char * _caption);
        Menu & addItemTitle(const char * _key, const char * _caption);
        Menu & setLastItemCallback(void (* _fn)(MenuItem * _self, void * _data), void * _data = NULL);
        Menu & setDestroyCallback(void (* _fn)(Menu * _self, void * _data), void * _data = NULL);
        Menu & position();

        void destroy();
        bool logic();
        void draw(GraphicPrinter & print);

        bool operator == (const Menu & _ref) const {
            return this == &_ref;
        }
};

#endif // MENU_H_INCLUDED
