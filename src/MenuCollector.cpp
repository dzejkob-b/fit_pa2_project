#include "MenuCollector.h"

#include <algorithm>

Menu & MenuCollector::createMenu() {

    this->menuItems.push_back(Menu(this));

    return this->menuItems.back();
}

void MenuCollector::destroyMenu(Menu & menuRef) {

    this->menuToDestroy.push_back(&menuRef);

}

void MenuCollector::destroyAll() {
    for (std::list<Menu>::iterator _it = this->menuItems.begin(); _it != this->menuItems.end(); ++_it) {

        (*_it).destroy();

    }
}

bool MenuCollector::anyHover() {
    for (std::list<Menu>::iterator _it = this->menuItems.begin(); _it != this->menuItems.end(); ++_it) {
        if ((*_it).anyHover()) {
            return true;
        }
    }

    return false;
}

/**
* process necessary real-time logic of all Menu in MenuCollector
*/
void MenuCollector::logic() {
    // finish menus destroying

    while (this->menuToDestroy.size() > 0) {
        std::list<Menu>::iterator _it = std::find(this->menuItems.begin(), this->menuItems.end(), *( this->menuToDestroy.back() ));

        if (_it != this->menuItems.end()) {
            this->menuItems.remove(*_it);
        }

        this->menuToDestroy.pop_back();
    }

    // process menus logic

    for (std::list<Menu>::reverse_iterator _it = this->menuItems.rbegin(); _it != this->menuItems.rend(); ++_it) {
        if ((*_it).logic()) {
            break;
        }
    }
}

void MenuCollector::draw(GraphicPrinter & print) {
    for (std::list<Menu>::iterator _it = this->menuItems.begin(); _it != this->menuItems.end(); ++_it) {
        (*_it).draw(print);
    }
}
