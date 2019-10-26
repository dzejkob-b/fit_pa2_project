#ifndef GRAPHICPRINTERBMP_H_INCLUDED
#define GRAPHICPRINTERBMP_H_INCLUDED

#include "GraphicPrinter.h"
#include "DrawArea.h"
#include "IBorder.h"
#include <fstream>

class GraphicPrinterBMP : public GraphicPrinter {
    private:
        int offsetX;
        int offsetY;
        int prefWidth;
        int prefHeight;
        int useWidth;

        int pixelDataSize;
        unsigned char * pixelData;

    public:
        GraphicPrinterBMP();
        ~GraphicPrinterBMP();

        int initialize(IBorder * _brd);
        int output(std::ofstream & _outStream);

        void drawPixel(Color & col, Point & pt);
};

#endif // GRAPHICPRINTERBMP_H_INCLUDED
