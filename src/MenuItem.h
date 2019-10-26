#ifndef MENUITEM_H_INCLUDED
#define MENUITEM_H_INCLUDED

#include <SDL2/SDL.h>
#include <string>

#include "GraphicPrinter.h"
#include "IRenderer.h"

class MenuCollector;

class Menu;

/**
* class representing textual item in Menu class
*/
class MenuItem {
    protected:
        Menu * parent;
        Menu * childMenu = NULL;

        std::string key;
        std::string caption;

        SDL_Surface * surRef = NULL;
        SDL_Texture * texRef = NULL;

        int textWidth;
        int textHeight;

        void (* clickCallback)(MenuItem * _self, void * _data) = NULL;
        void * clickData = NULL;

        void updateTexture();

    public:
        MenuItem(Menu * _parent, const char * _key, const char * _caption);
        virtual ~MenuItem();

        Menu * getParent();
        MenuCollector * getCollector();
        void setChildMenu(Menu * _childMenu);

        std::string & getKey();
        std::string & getCaption();

        void setClickCallback(void (* _fn)(MenuItem * _self, void * _data), void * _data = NULL);
        void doClick();

        virtual bool isHoverable();
        virtual bool isClickable();

        virtual void measure(int & width, int & height);
        virtual void draw(GraphicPrinter & print, int & posX, int & posY, int maxWidth);
};

#endif // MENUITEM_H_INCLUDED
