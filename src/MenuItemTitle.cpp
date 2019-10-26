#include "MenuItemTitle.h"
#include "Services.h"

MenuItemTitle::MenuItemTitle(Menu * _parent, const char * _key, const char * _caption) : MenuItem(_parent, _key, _caption) {
}

MenuItemTitle::~MenuItemTitle() {
}

bool MenuItemTitle::isHoverable() {
    return false;
}

bool MenuItemTitle::isClickable() {
    return false;
}

void MenuItemTitle::draw(GraphicPrinter & print, int & posX, int & posY, int maxWidth) {

    Rect re;

    re.x = posX;
    re.y = posY;
    re.w = maxWidth;
    re.h = 28;

    Color col(140, 7, 7);

    print.drawFilledRectangle(col, re);

    col.setCol(107, 6, 6);

    print.drawLine(col, re.x, re.y + re.h, re.x + re.w, re.y + re.h);
    print.drawLine(col, re.x + re.w, re.y, re.x + re.w, re.y + re.h);

    re.x = posX + 5;
    re.y = posY + 3;

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
