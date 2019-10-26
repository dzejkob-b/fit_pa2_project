#include "DrawEntity.h"
#include "DrawFiller.h"
#include "Services.h"

DrawFiller::DrawFiller(DrawEntity * _entity) {
    this->entity = _entity;

    this->setStyle(DrawFiller::lastUseStyle);
    this->setColor(DrawFiller::lastUseColor);
}

DrawFiller::~DrawFiller() {
}

int DrawFiller::lastUseStyle = FillStyle::STYLE_LINECROSS;
Color DrawFiller::lastUseColor = Color(120, 120, 120);

const int DrawFiller::getStyle() const {
    return this->useStyle;
}

const Color & DrawFiller::getColor() const {
    return this->useColor;
}

void DrawFiller::setStyle(int _useStyle) {

    DrawFiller::lastUseStyle = (this->useStyle = _useStyle);

    switch (this->useStyle) {
        case FillStyle::STYLE_SOLID :
            this->callReset = [](DrawFiller * _self) {
                _self->rayDir.addVals(10, 0);
            };

            this->callStep = [](DrawFiller * _self) {
                _self->rayFrom.addVals(0, 1);
            };
            break;

        case FillStyle::STYLE_LINED45 :
            this->callReset = [](DrawFiller * _self) {
                _self->rayDir.addRadius(- M_PI / 4, 10);
            };

            this->callStep = [](DrawFiller * _self) {
                _self->rayFrom.addVals(0, 10);
            };
            break;

        case FillStyle::STYLE_LINED45_REV :
            this->callReset = [](DrawFiller * _self) {
                _self->rayDir.addRadius(1 * (M_PI / 4), 10);
            };

            this->callStep = [](DrawFiller * _self) {
                _self->rayFrom.addVals(0, 10);
            };
            break;

        case FillStyle::STYLE_LINECROSS :

            break;
    }

}

void DrawFiller::setColor(Color _useColor) {

    DrawFiller::lastUseColor = (this->useColor = _useColor);

}

void DrawFiller::resetRay() {

    switch (this->useStyle) {
        case FillStyle::STYLE_LINED45_REV :
            this->rayFrom = Point(this->entity->i_getRightTop());
            break;

        default :
            this->rayFrom = Point(this->entity->i_getLeftTop());
            break;
    }

    this->rayDir = Point(this->rayFrom);
    this->callReset(this);

}

void DrawFiller::rayStep() {

    this->callStep(this);

    this->rayDir = Point(this->rayFrom);
    this->callReset(this);

}

void DrawFiller::fillLogic(GraphicPrinter & print) {

    int safe = 0;
    bool entry = false;
    int entryCnt = 0;

    while (safe < 1000) {
        safe++;

        std::vector<Point> lst = this->entity->collidePoints(this->rayFrom, this->rayDir);

        if (lst.size() > 0 && lst.size() % 2 == 0) {
            unsigned int pos = 0;

            while (pos < lst.size()) {
                Point * ptFrom = &lst[pos];
                Point * ptTo = &lst[pos + 1];

                print.drawLine(this->useColor, *ptFrom, *ptTo);

                pos += 2;
            }

            entry = true;
            entryCnt = 2;

        } else if (entry) {
            entryCnt--;

            if (entryCnt == 0) {
                break;
            }
        }

        this->rayStep();

    }
}

void DrawFiller::drawFill(GraphicPrinter & print) {

    if (this->useStyle == FillStyle::STYLE_NONE) {
        // skip ...

    } else if (this->useStyle == FillStyle::STYLE_LINECROSS) {

        this->setStyle(FillStyle::STYLE_LINED45);
        this->resetRay();
        this->fillLogic(print);

        this->setStyle(FillStyle::STYLE_LINED45_REV);
        this->resetRay();
        this->fillLogic(print);

        this->setStyle(FillStyle::STYLE_LINECROSS);

    } else {
        this->resetRay();
        this->fillLogic(print);
    }

}
