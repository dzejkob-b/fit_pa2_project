#ifndef GRAPHICPRINTERSDL_H_INCLUDED
#define GRAPHICPRINTERSDL_H_INCLUDED

#include "GraphicPrinter.h"
#include "SDL2/SDL.h"

/**
* graphic output directly to screen via SDL
*/
class GraphicPrinterSDL : public GraphicPrinter {
    private:
        SDL_Renderer * rndRef;
        SDL_Rect tmpRe;

    public:
        GraphicPrinterSDL();
        ~GraphicPrinterSDL();

        void drawPixel(Color & col, Point & pt);
        void drawLine(Color & col, Point & ptFrom, Point & ptTo);
        void drawRectangle(Color & col, Rect & re);
        void drawFilledRectangle(Color & col, Rect & re);
};

#endif // GRAPHICPRINTERSDL_H_INCLUDED
