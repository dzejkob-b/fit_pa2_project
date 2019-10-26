#ifndef PROGRAM_H_INCLUDED
#define PROGRAM_H_INCLUDED

#include <iostream>
#include <sstream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "IRenderer.h"
#include "IMouse.h"
#include "MenuCollector.h"
#include "UserControl.h"
#include "DrawArea.h"
#include "GraphicPrinterSDL.h"

/**
* main Program class providing startup and initialization logic
* and references of necessary components
*/
class Program : public IRenderer, public IMouse {
    private:
        SDL_Window          * wndRef = NULL;
        SDL_Renderer        * renderRef = NULL;
        TTF_Font            * fontRef = NULL;
        GraphicPrinterSDL   * psdl = NULL;
        SDL_Event             event;

        MenuCollector         menu;
        UserControl           control;
        DrawArea              area;

        bool loop = false;
        std::stringstream logStream;

        int mouseX = 0;
        int mouseY = 0;
        int mouseButton = 0;

    public:
        Program();
        ~Program();

        MenuCollector * getMenu();
        DrawArea * getArea();

        void logPush(const char * str = NULL);

        int initScene(const char * sceneName);
        int setupSDL();
        int runSDL();
        int quitSDL();

        // IRenderer

        SDL_Renderer        * i_getRenderRef();
        TTF_Font            * i_getFontRef();
        void                  i_getRenderAreaSize(int & width, int & height);
        void                  i_logPush(const char * str = NULL);
        std::stringstream   & i_logStream();

        // IMouse

        int                   i_getMouseX();
        int                   i_getMouseY();
        int                   i_getMouseButton();
        SDL_Event             i_getEvent();
        void                  i_setEventUsed();

};

#endif // PROGRAM_H_INCLUDED
