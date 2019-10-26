#include "MenuItem.h"
#include "Menu.h"
#include "MenuCollector.h"
#include "Rect.h"
#include "Services.h"

MenuItem::MenuItem(Menu * _parent, const char * _key, const char * _caption) {

    this->parent = _parent;
    this->key = std::string(_key);
    this->caption = std::string(_caption);

    this->updateTexture();

}

MenuItem::~MenuItem() {
    if (this->texRef != NULL) {
        SDL_DestroyTexture(this->texRef);
        this->texRef = NULL;
    }
}

Menu * MenuItem::getParent() {
    return this->parent;
}

MenuCollector * MenuItem::getCollector() {
    return this->parent->getParent();
}

void MenuItem::setChildMenu(Menu * _childMenu) {
    if (this->childMenu != NULL) {
        this->childMenu->destroy();
        this->childMenu = NULL;
    }

    this->childMenu = _childMenu;
}

std::string & MenuItem::getKey() {
    return this->key;
}

std::string & MenuItem::getCaption() {
    return this->caption;
}

void MenuItem::setClickCallback(void (* _fn)(MenuItem * _self, void * _data), void * _data) {
    this->clickCallback = _fn;
    this->clickData = _data;
}

/**
* process click event callback
*/
void MenuItem::doClick() {
    if (this->clickCallback != NULL) {
        this->clickCallback(this, this->clickData);
    }
}

bool MenuItem::isHoverable() {
    return this->clickCallback != NULL;
}

bool MenuItem::isClickable() {
    return this->clickCallback != NULL;
}

/**
* update text texture - must be called after creation or text update
*/
void MenuItem::updateTexture() {

    if (this->texRef != NULL) {
        SDL_DestroyTexture(this->texRef);
        this->texRef = NULL;
    }

    IRenderer * rnd = Services::getRef()->getRnd();

    this->surRef = TTF_RenderText_Solid(rnd->i_getFontRef(), this->caption.c_str(), SDL_Color({255, 255, 255}));
    this->texRef = SDL_CreateTextureFromSurface(rnd->i_getRenderRef(), this->surRef);

    SDL_Rect re;
    SDL_QueryTexture(this->texRef, NULL, NULL, &re.w, &re.h);

    this->textWidth = re.w;
    this->textHeight = re.h;

}

/**
* get width and height of MenuItem
* @param width width by ref
* @param height height by ref
*/
void MenuItem::measure(int & width, int & height) {

    width = this->textWidth + 10;
    height = 29;

}

void MenuItem::draw(GraphicPrinter & print, int & posX, int & posY, int maxWidth) {

    int st = this->parent->getItemState(this);

    Color col;

    if (st == 2) {
        col.setCol(70, 70, 70);
    } else if (st == 1) {
        col.setCol(150, 150, 150);
    } else {
        col.setCol(90, 90, 90);
    }

    Rect re;

    re.x = posX;
    re.y = posY;
    re.w = maxWidth;
    re.h = 28;

    print.drawFilledRectangle(col, re);

    col.setCol(60, 60, 60);
    print.drawLine(col, re.x, re.y + re.h, re.x + re.w, re.y + re.h);
    print.drawLine(col, re.x + re.w, re.y, re.x + re.w, re.y + re.h);

    col.setCol(110, 110, 110);
    print.drawLine(col, re.x, re.y, re.x + re.w - 1, re.y);
    print.drawLine(col, re.x, re.y, re.x, re.y + re.h - 1);

    re.x = posX + 5;
    re.y = posY + 3;

    if (st == 2) {
        re.x += 2;
        re.y += 2;
    }

    // sdl texture put

    IRenderer * rnd = Services::getRef()->getRnd();

    SDL_Rect sRect;

    sRect.x = re.x;
    sRect.y = re.y;
    sRect.w = re.w;
    sRect.h = re.h;

    SDL_QueryTexture(this->texRef, NULL, NULL, &sRect.w, &sRect.h);
    SDL_RenderCopy(rnd->i_getRenderRef(), this->texRef, NULL, &sRect);

    posY += 29;
}
