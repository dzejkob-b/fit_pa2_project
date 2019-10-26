#include "GraphicPrinterBMP.h"
#include <stdio.h>
#include <string.h>
#include <iostream>

GraphicPrinterBMP::GraphicPrinterBMP() {
    this->prefWidth = 0;
    this->prefHeight = 0;
    this->offsetX = 0;
    this->offsetY = 0;

    this->pixelDataSize = 0;
    this->pixelData = NULL;
}

GraphicPrinterBMP::~GraphicPrinterBMP() {
    if (this->pixelData != NULL) {
        free(this->pixelData);
        this->pixelData = NULL;
    }
}

/**
* initialize bitmap parameters, size and data
*/
int GraphicPrinterBMP::initialize(IBorder * _brd) {
    int result = 0;

    Point leftTop = _brd->i_getLeftTop();
    Point rightBottom = _brd->i_getRightBottom();

    this->offsetX = leftTop.getX() - 1;
    this->offsetY = leftTop.getY() - 1;

    this->prefWidth = rightBottom.getX() - leftTop.getX() + 3;
    this->prefHeight = rightBottom.getY() - leftTop.getY() + 3;

    this->useWidth = this->prefWidth * 3;

    if (this->useWidth % 4 != 0) {
        this->useWidth += 4 - (this->useWidth % 4); // line must be dividable by 4
    }

    this->pixelDataSize = this->useWidth * this->prefHeight;
    this->pixelData = (unsigned char *)malloc(sizeof(unsigned char) * this->pixelDataSize);

    for (int sf = 0; sf < this->pixelDataSize; sf++) {
        this->pixelData[sf] = 0;
    }

    return result;
}

/**
* output initialized bitmap to stream
* for valid result all objects must be drawn to this printer
*/
int GraphicPrinterBMP::output(std::ofstream & _outStream) {
    int result = 0;

    if (this->pixelData == NULL) {
        return -2;
    }

    short bits = 24;
    int sz = 40 + bits * (this->prefWidth * this->prefHeight);
    int blankInt = 0;
    short blank = 0;

    _outStream << 'B' << 'M'; // bfType | must always be set to BM

    _outStream.write((char *)(&sz), sizeof(sz)); // bfSize | specifies the size of the file in bytes
    _outStream.write((char *)(&blank), sizeof(blank)); // bfReserved1 | must always be set to zero
    _outStream.write((char *)(&blank), sizeof(blank)); // bfReserved2 | must always be set to zero

    sz = 54;

    _outStream.write((char *)(&sz), sizeof(sz)); // bfOffBits | specifies the offset from the beginning of the file to the bitmap data

    sz = 40;

    _outStream.write((char *)(&sz), sizeof(sz)); // biSize | specifies the size of the BITMAPINFOHEADER structure, in bytes
    _outStream.write((char *)(&this->prefWidth), sizeof(this->prefWidth)); // biWidth | specifies the width of the image, in pixels
    _outStream.write((char *)(&this->prefHeight), sizeof(this->prefHeight)); // biHeight | specifies the height of the image, in pixels
    _outStream.write((char *)(&blank), sizeof(blank)); // biPlanes | zero
    _outStream.write((char *)(&bits), sizeof(bits)); // biBitCount | specifies the number of bits per pixel
    _outStream.write((char *)(&blankInt), sizeof(blankInt)); // biCompression
    _outStream.write((char *)(&blankInt), sizeof(blankInt)); // biSizeImage
    _outStream.write((char *)(&blankInt), sizeof(blankInt)); // biXPelsPerMeter
    _outStream.write((char *)(&blankInt), sizeof(blankInt)); // biYPelsPerMeter
    _outStream.write((char *)(&blankInt), sizeof(blankInt)); // biClrUsed
    _outStream.write((char *)(&blankInt), sizeof(blankInt)); // biClrImportant

    if (_outStream.fail()) {
        return -1;
    }

    _outStream.write((const char *)this->pixelData, sizeof(char) * this->pixelDataSize);

    if (_outStream.fail()) {
        result = -1;
    }

    return result;
}

void GraphicPrinterBMP::drawPixel(Color & col, Point & pt) {

    // bottom line is on the top

    int tstX = pt.getIntX() - this->offsetX;
    int tstY = pt.getIntY() - this->offsetY;

    if (tstX >= 0 && tstY >= 0 && tstX < this->prefWidth && tstY < this->prefHeight) {

        int idx = ((this->prefHeight - tstY - 1) * this->useWidth) + (tstX * 3);

        this->pixelData[idx] = (unsigned char)col.b;
        this->pixelData[idx + 1] = (unsigned char)col.g;
        this->pixelData[idx + 2] = (unsigned char)col.r;

    }
}
