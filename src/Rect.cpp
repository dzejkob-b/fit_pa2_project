#include "Rect.h"

void Rect::setSDLRect(SDL_Rect & re) {

    re.x = this->x;
    re.y = this->y;
    re.w = this->w;
    re.h = this->h;

}
