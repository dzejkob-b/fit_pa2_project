#ifndef IRENDERER_H_INCLUDED
#define IRENDERER_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <sstream>

/**
* interface providing basic shared features
*/
class IRenderer {
    public:

        virtual ~IRenderer() {}
        virtual SDL_Renderer * i_getRenderRef() = 0;
        virtual TTF_Font * i_getFontRef() = 0;
        virtual void i_getRenderAreaSize(int & width, int & height) = 0;
        virtual void i_logPush(const char * str = NULL) = 0;
        virtual std::stringstream & i_logStream() = 0;

};

#endif // IRENDERER_H_INCLUDED
