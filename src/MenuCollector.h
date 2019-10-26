#ifndef MENUCOLLECTOR_H_INCLUDED
#define MENUCOLLECTOR_H_INCLUDED

#include <list>
#include "GraphicPrinter.h"
#include "Menu.h"

/**
* main class which managing all opened menus
*/
class MenuCollector {
    private:
        std::list<Menu> menuItems;
        std::list<Menu *> menuToDestroy;

    public:
        Menu & createMenu();
        void destroyMenu(Menu & menuRef);
        void destroyAll();

        bool anyHover();

        void logic();
        void draw(GraphicPrinter & print);
};

#endif // MENUCOLLECTOR_H_INCLUDED
