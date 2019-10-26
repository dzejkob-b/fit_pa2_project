#include "Program.h"
#include "Services.h"
#include "GraphicPrinterSDL.h"

#include <string>
#include <sstream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

Program::Program() : control(&menu, &area) {
}

Program::~Program() {
}

MenuCollector * Program::getMenu() {
    return &this->menu;
}

DrawArea * Program::getArea() {
    return &this->area;
}

/**
* load scene or init scene (if scene with that name is not existing)
*/
int Program::initScene(const char * sceneName) {
    return this->area.loadTextFromFile(sceneName);
}

int Program::setupSDL() {
    if (SDL_Init(SDL_INIT_EVERYTHING) >= 0) {

        Services::getRef()->setRnd(this);
        Services::getRef()->setMouse(this);

        if ((this->wndRef = SDL_CreateWindow("hrabajak draw", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768, SDL_WINDOW_SHOWN)) == NULL) {
            this->logPush("Cannot create window!");
            return -1;
        }

        if ((this->renderRef = SDL_CreateRenderer(this->wndRef, -1, 0)) == NULL) {
            this->logPush("Cannot create renderer!");
            return -2;
        }

        if (TTF_Init() < 0) {
            this->logStream << "Cannot init font library: " << TTF_GetError();
            this->logPush();
            return -3;
        }

        if ((this->fontRef = TTF_OpenFont("examples/dali.ttf", 20)) == NULL) {
            this->logStream << "Cannot load font: " << TTF_GetError();
            this->logPush();
            return -4;
        }

        SDL_SetRenderDrawBlendMode(this->renderRef, SDL_BLENDMODE_BLEND);

        return 0;

    } else {
        this->logStream << "SDL init failed: " << SDL_GetError();
        this->logPush();
        return -5;
    }
}

int Program::runSDL() {
    int result = 0;

    this->loop = true;
    this->psdl = new GraphicPrinterSDL();

    unsigned int lastTime = 0;

    while (this->loop) {

        SDL_PumpEvents();
        SDL_GetMouseState(&this->mouseX, &this->mouseY);

        if (SDL_PollEvent(&this->event) >= 0) {
            switch (this->event.type) {
                case SDL_QUIT:
                    this->loop = false;
                    break;

                case SDL_KEYDOWN :

                    switch (this->event.key.keysym.sym) {
                        case SDLK_ESCAPE :
                            this->loop = false;
                            break;
                    }

                    break;

                case SDL_MOUSEBUTTONDOWN :
                    this->mouseButton = 0;

                    if (this->event.button.button == SDL_BUTTON_LEFT) {
                        this->mouseButton = 1;

                    } else if (this->event.button.button == SDL_BUTTON_RIGHT) {
                        this->mouseButton = 2;
                    }
                    break;

                case SDL_MOUSEBUTTONUP :
                    break;
            }
        }

        // logic

        this->area.logic();
        this->menu.logic();
        this->control.logic();

        this->event.type = 0;

        // draw logic

        if (SDL_GetTicks() - lastTime > 50) {

            SDL_SetRenderDrawColor(this->renderRef, 0, 0, 0, 255);
            SDL_RenderClear(this->renderRef);

            this->area.draw(*this->psdl);
            this->control.draw(*this->psdl);
            this->menu.draw(*this->psdl);

            SDL_RenderPresent(this->renderRef);

            lastTime = SDL_GetTicks();
        }

        SDL_Delay(5);
    }

    return result;
}

int Program::quitSDL() {

    if (this->psdl != NULL) {
        delete this->psdl;
        this->psdl = NULL;
    }

    if (this->renderRef != NULL) {
        SDL_DestroyRenderer(this->renderRef);
    }

    if (this->wndRef != NULL) {
        SDL_DestroyWindow(this->wndRef);
    }

    SDL_Quit();

    return 0;
}

void Program::logPush(const char * str) {
    if (str != NULL) {
        this->logStream << str;
    }

    this->logStream << "\n";
    // !! SDL_Log(this->logStream.str().c_str());
    std::cout << this->logStream.str().c_str();
    this->logStream.str("");
}

SDL_Renderer * Program::i_getRenderRef() { return this->renderRef; }

TTF_Font * Program::i_getFontRef() { return this->fontRef; }

void Program::i_getRenderAreaSize(int & width, int & height) {
    SDL_GetWindowSize(this->wndRef, &width, &height);
}

void Program::i_logPush(const char * str) {
    this->logPush(str);
}

std::stringstream & Program::i_logStream() {
    return this->logStream;
}

int Program::i_getMouseX() {
    return this->mouseX;
}

int Program::i_getMouseY() {
    return this->mouseY;
}

int Program::i_getMouseButton() {
    return this->mouseButton;
}

SDL_Event Program::i_getEvent() {
    return this->event;
}

void Program::i_setEventUsed() {
    this->event.type = 0;
}
