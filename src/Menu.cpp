#include "Menu.h"
#include "Services.h"
#include "MenuCollector.h"

#include <SDL2/SDL.h>

Menu::Menu(MenuCollector * _parent) {
    this->parent = _parent;
}

Menu::~Menu() {
    for (std::vector<MenuItem *>::iterator _it = this->items.begin(); _it != this->items.end(); ++_it) {
        delete *_it;
    }
}

MenuCollector * Menu::getParent() {
    return this->parent;
}

int Menu::getCoordX() const {
    return this->coordX;
}

int Menu::getCoordY() const {
    return this->coordY;
}

/**
* @return get MenuItem state: 0 - not touched, 1 - hover, 2 - pressed down
*/
int Menu::getItemState(MenuItem * _item) {
    if (_item == this->itemDown) {
        return 2;
    } else if (_item == this->itemHover) {
        return 1;
    } else {
        return 0;
    }
}

/**
* update coords of given MenuItem
* @param cX ref of X coord
* @param cY ref of Y coord
*/
void Menu::getItemCoords(MenuItem * _findItem, int & cX, int & cY) {
    cX = this->coordX + this->maxWidth;
    cY = this->coordY;

    MenuItem * item = NULL;

    for (std::vector<MenuItem *>::iterator _it = this->items.begin(); _it != this->items.end(); ++_it) {
        int tW, tH;

        item = *_it;
        item->measure(tW, tH);

        if (item == _findItem) {
            break;
        }

        cY += tH;
    }
}

bool Menu::anyHover() {
    return this->itemHover != NULL;
}

Menu & Menu::setCoords(int _coordX, int _coordY) {
    this->coordX = _coordX;
    this->coordY = _coordY;

    return *this;
}

Menu & Menu::setParentItem(MenuItem * _parentItem) {

    _parentItem->getParent()->getItemCoords(_parentItem, this->coordX, this->coordY);
    _parentItem->setChildMenu(this);

    return *this;
}

Menu & Menu::addItem(const char * _key, const char * _caption) {

    this->items.push_back(this->itemLast = new MenuItem(this, _key, _caption));

    return *this;
}

Menu & Menu::addItemTitle(const char * _key, const char * _caption) {

    this->items.push_back(this->itemLast = new MenuItemTitle(this, _key, _caption));

    return *this;
}

Menu & Menu::setLastItemCallback(void (* _fn)(MenuItem * _self, void * _data), void * _data) {

    this->itemLast->setClickCallback(_fn, _data);

    return *this;
}

Menu & Menu::setDestroyCallback(void (* _fn)(Menu * _self, void * _data), void * _data) {

    this->destroyCallback = _fn;
    this->destroyData = _data;

    return *this;
}

/**
* update position and fix if overflows screen
*/
Menu & Menu::position() {

    IRenderer * rnd = Services::getRef()->getRnd();

    this->maxHeight = 0;
    this->maxWidth = 0;

    MenuItem * item = NULL;

    for (std::vector<MenuItem *>::iterator _it = this->items.begin(); _it != this->items.end(); ++_it) {
        int tW, tH;

        item = *_it;
        item->measure(tW, tH);

        if (tW > this->maxWidth) {
            this->maxWidth = tW;
        }

        this->maxHeight += tH;
    }

    int areaWidth, areaHeight;

    rnd->i_getRenderAreaSize(areaWidth, areaHeight);

    if (this->coordY + this->maxHeight > areaHeight) {
        this->coordY = areaHeight - this->maxHeight - 10;
    }

    if (this->coordX + this->maxWidth > areaWidth) {
        this->coordX = areaWidth - this->maxWidth - 10;
    }

    return *this;
}

void Menu::dropChildMenus() {
    for (std::vector<MenuItem *>::iterator _it = this->items.begin(); _it != this->items.end(); ++_it) {
        (*_it)->setChildMenu(NULL);
    }
}

void Menu::destroy() {
    this->dropChildMenus();

    if (this->destroyCallback != NULL) {
        this->destroyCallback(this, this->destroyData);
    }

    this->parent->destroyMenu(*this);
}

/**
* process necessary real-time Menu logic (hovering, clicking etc.)
*/
bool Menu::logic() {
    bool result = false;

    IMouse * mouse = Services::getRef()->getMouse();

    int posX = this->coordX;
    int posY = this->coordY;

    MenuItem * item = NULL;

    this->itemHover = NULL;

    for (std::vector<MenuItem *>::iterator _it = this->items.begin(); _it != this->items.end(); ++_it) {
        int tW, tH;

        item = *_it;
        item->measure(tW, tH);

        if (
            item->isHoverable() &&
            mouse->i_getMouseX() >= posX && mouse->i_getMouseY() >= posY &&
            mouse->i_getMouseX() < posX + this->maxWidth && mouse->i_getMouseY() < posY + tH
        ) {

            if (mouse->i_getEvent().type == SDL_MOUSEBUTTONDOWN) {
                this->itemDown = item;

                mouse->i_setEventUsed();
            }

            this->itemHover = item;
            result = true;
        }

        posY += tH;
    }

    if (mouse->i_getEvent().type == SDL_MOUSEBUTTONUP && this->itemDown != NULL && this->itemHover == this->itemDown) {
        this->dropChildMenus();
        this->itemDown->doClick();
        this->itemDown = NULL;

        mouse->i_setEventUsed();
        result = true;

    } else if (this->itemHover == NULL) {
        this->itemDown = NULL;
    }

    return result;
}

void Menu::draw(GraphicPrinter & print) {

    int posX = this->coordX;
    int posY = this->coordY;

    int offs = 6;

    Rect re;

    re.x = this->coordX - 2 + offs;
    re.y = this->coordY - 2 + offs;
    re.w = this->maxWidth + 5;
    re.h = this->maxHeight + 4;

    Color col(50, 50, 50, 120);

    print.drawFilledRectangle(col, re);

    re.x -= offs;
    re.y -= offs;

    col.setCol(180, 180, 180);
    print.drawFilledRectangle(col, re);

    re.x += 1;
    re.y += 1;
    re.w -= 2;
    re.h -= 2;

    col.setCol(20, 20, 20);
    print.drawFilledRectangle(col, re);

    MenuItem * item = NULL;

    for (std::vector<MenuItem *>::iterator _it = this->items.begin(); _it != this->items.end(); ++_it) {

        item = *_it;
        item->draw(print, posX, posY, this->maxWidth);

    }
}
