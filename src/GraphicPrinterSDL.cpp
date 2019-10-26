#include "GraphicPrinterSDL.h"
#include "Services.h"
#include "Point.h"

GraphicPrinterSDL::GraphicPrinterSDL() {
    this->rndRef = Services::getRef()->getRnd()->i_getRenderRef();
}

GraphicPrinterSDL::~GraphicPrinterSDL() {
}

void GraphicPrinterSDL::drawPixel(Color & col, Point & pt) {

    SDL_SetRenderDrawColor(this->rndRef, col.r, col.g, col.b, col.a);
    SDL_RenderDrawPoint(this->rndRef, pt.getIntX(), pt.getIntY());

}

void GraphicPrinterSDL::drawLine(Color & col, Point & ptFrom, Point & ptTo) {

    SDL_SetRenderDrawColor(this->rndRef, col.r, col.g, col.b, col.a);
    SDL_RenderDrawLine(this->rndRef, ptFrom.getIntX(), ptFrom.getIntY(), ptTo.getIntX(), ptTo.getIntY());

}

void GraphicPrinterSDL::drawRectangle(Color & col, Rect & re) {

    re.setSDLRect(this->tmpRe);

    SDL_SetRenderDrawColor(this->rndRef, col.r, col.g, col.b, col.a);
    SDL_RenderDrawRect(this->rndRef, &this->tmpRe);
}

void GraphicPrinterSDL::drawFilledRectangle(Color & col, Rect & re) {

    re.setSDLRect(this->tmpRe);

    SDL_SetRenderDrawColor(this->rndRef, col.r, col.g, col.b, col.a);
    SDL_RenderFillRect(this->rndRef, &this->tmpRe);
}
