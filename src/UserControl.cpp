#include "UserControl.h"
#include "Services.h"
#include "DrawShortcut.h"
#include <iostream>

UserControl::UserControl(MenuCollector * _menu, DrawArea * _area) {
    this->menu = _menu;
    this->area = _area;
}

UserControl::~UserControl() {
}

void UserControl::logic() {

    IMouse * mouse = Services::getRef()->getMouse();

    if (mouse->i_getEvent().type == SDL_MOUSEBUTTONUP && mouse->i_getMouseButton() == 2) {

        switch (this->state) {
            case UserControl::STATE_NONE :
            case UserControl::STATE_MODIFY :
                this->manNode = this->area->getNodeByCoords(Point(mouse->i_getMouseX(), mouse->i_getMouseY()));

                if (this->manNode != NULL) {
                    this->stateTransfer(UserControl::STATE_MODIFY);

                } else {
                    this->stateTransfer(UserControl::STATE_NONE);
                    this->createRootMenu();
                }
                break;

            case UserControl::STATE_POLYLINE :
            case UserControl::STATE_SQUARE :
            case UserControl::STATE_RECTANGLE :
            case UserControl::STATE_CIRCLE :
            case UserControl::STATE_ELLIPSE :
                this->createCreatingMenu();
                break;

            default :
                break;
        }

    } else if (mouse->i_getEvent().type == SDL_MOUSEBUTTONUP && mouse->i_getMouseButton() == 1) {

        switch (this->state) {
            case UserControl::STATE_NONE :
                if (!this->menu->anyHover()) {
                    if (this->rootMenu != NULL) {
                        this->rootMenu->destroy();
                        this->rootMenu = NULL;

                        mouse->i_setEventUsed();
                    }
                }
                break;

            case UserControl::STATE_POLYLINE :
            case UserControl::STATE_SQUARE :
            case UserControl::STATE_RECTANGLE :
            case UserControl::STATE_CIRCLE :
            case UserControl::STATE_ELLIPSE :
                if (!this->menu->anyHover()) {
                    if (this->creatingMenu != NULL) {
                        this->creatingMenu->destroy();
                        this->creatingMenu = NULL;

                        mouse->i_setEventUsed();
                    }
                }
                break;

            case UserControl::STATE_MODIFY :
                if (!this->menu->anyHover()) {
                    if (this->modifyMenu != NULL) {
                        this->stateTransfer(UserControl::STATE_NONE);

                        mouse->i_setEventUsed();
                    }
                }
                break;

            case UserControl::STATE_MOVE :
                if (this->moveFlag == 1) {

                    this->manNode->applyTranslation();
                    this->manNode->rebuild();

                    this->moveFlag = 0;
                    this->stateTransfer(UserControl::STATE_NONE);

                }
                break;
        }

    } else if (mouse->i_getEvent().type == SDL_MOUSEBUTTONDOWN && mouse->i_getMouseButton() == 1 && this->state == UserControl::STATE_MOVE) {

        this->moveFlag = 1;
        this->moveOffset.setVals(mouse->i_getMouseX(), mouse->i_getMouseY());

    }

    bool msClick = mouse->i_getEvent().type == SDL_MOUSEBUTTONUP && mouse->i_getMouseButton() == 1;

    switch (this->state) {
        case UserControl::STATE_MOVE :

            if (this->moveFlag == 1) {
                this->manNode->getTranslate().setOffset(
                    mouse->i_getMouseX() - this->moveOffset.getX(),
                    mouse->i_getMouseY() - this->moveOffset.getY()
                );

                //this->manNode->rebuild();
            }
            break;

        case UserControl::STATE_POLYLINE :

            if (!this->drawHelp.havePoint()) {

                if (msClick) {
                    this->drawHelp.addPoint({mouse->i_getMouseX(), mouse->i_getMouseY()});
                    mouse->i_setEventUsed();
                }

            } else {
                this->drawHelp.setLinePoint({mouse->i_getMouseX(), mouse->i_getMouseY()});

                if (msClick) {
                    this->drawHelp.stackFinalPoints();
                    this->drawHelp.clearPoints(true);
                    mouse->i_setEventUsed();
                }
            }
            break;

        case UserControl::STATE_SQUARE :

            if (!this->drawHelp.havePoint()) {

                if (msClick) {
                    this->drawHelp.addPoint({mouse->i_getMouseX(), mouse->i_getMouseY()});
                    mouse->i_setEventUsed();
                }

            } else {
                this->drawHelp.setRectanglePoint({mouse->i_getMouseX(), mouse->i_getMouseY()}, true);

                if (msClick) {
                    this->drawHelp.stackFinalPoints();
                    this->drawHelp.clearPoints();
                    mouse->i_setEventUsed();
                }
            }
            break;

        case UserControl::STATE_RECTANGLE :

            if (!this->drawHelp.havePoint()) {

                if (msClick) {
                    this->drawHelp.addPoint({mouse->i_getMouseX(), mouse->i_getMouseY()});
                    mouse->i_setEventUsed();
                }

            } else {
                this->drawHelp.setRectanglePoint({mouse->i_getMouseX(), mouse->i_getMouseY()});

                if (msClick) {
                    this->drawHelp.stackFinalPoints();
                    this->drawHelp.clearPoints();
                    mouse->i_setEventUsed();
                }
            }
            break;

        case UserControl::STATE_CIRCLE :

            if (!this->drawHelp.havePoint()) {

                if (msClick) {
                    this->drawHelp.addPoint({mouse->i_getMouseX(), mouse->i_getMouseY()});
                    mouse->i_setEventUsed();
                }

            } else {
                this->drawHelp.setRectanglePoint({mouse->i_getMouseX(), mouse->i_getMouseY()}, true);

                if (msClick) {
                    this->drawHelp.stackFinalPoints();
                    this->drawHelp.clearPoints();
                    mouse->i_setEventUsed();
                }
            }
            break;

        case UserControl::STATE_ELLIPSE :

            if (!this->drawHelp.havePoint()) {

                if (msClick) {
                    this->drawHelp.addPoint({mouse->i_getMouseX(), mouse->i_getMouseY()});
                    mouse->i_setEventUsed();
                }

            } else {
                this->drawHelp.setRectanglePoint({mouse->i_getMouseX(), mouse->i_getMouseY()}, false);

                if (msClick) {
                    this->drawHelp.stackFinalPoints();
                    this->drawHelp.clearPoints();
                    mouse->i_setEventUsed();
                }
            }
            break;

        default :
            // ...

            break;
    }

}

void UserControl::createRootMenu() {

    if (this->rootMenu != NULL) {
        this->rootMenu->destroy();
        this->rootMenu = NULL;
    }

    IMouse * mouse = Services::getRef()->getMouse();

    this->rootMenu = &this->menu->createMenu()

    .setCoords(mouse->i_getMouseX(), mouse->i_getMouseY())
    .addItemTitle("title", "... main menu")

    .addItem("scene", "... scene")
    .setLastItemCallback([](MenuItem * _self, void * _data) {

        ((UserControl *)_data)->manNode = ((UserControl *)_data)->area->getGroup("scene");
        ((UserControl *)_data)->stateTransfer(UserControl::STATE_MODIFY);

    }, this)

    .addItem("save", "... save")
    .setLastItemCallback([](MenuItem * _self, void * _data) {

        _self->getCollector()->createMenu()
        .setParentItem(_self)
        .addItemTitle("tit", std::string("Scene name: ").append(((UserControl *)_data)->area->getScenePath()).c_str())
        .addItem("text", "Save as text")
        .setLastItemCallback([](MenuItem * _self, void * _data) {

            ((UserControl *)_data)->area->saveText();

        }, _data)
        .addItem("bitmap", "Save as bitmap (BMP)")
        .setLastItemCallback([](MenuItem * _self, void * _data) {

            ((UserControl *)_data)->area->saveBitmap();

        }, _data)
        .position();

    }, this)

    .setDestroyCallback([](Menu * _self, void * _data) {

        ((UserControl *)_data)->rootMenu = NULL;

    }, this)

    .addItem("close", "Cancel")
    .setLastItemCallback([](MenuItem * _self, void * _data) {

        _self->getParent()->destroy();

    })
    .position();
}

void UserControl::createCreatingMenu() {
    if (this->creatingMenu != NULL) {
        this->creatingMenu->destroy();
        this->creatingMenu = NULL;
    }

    IMouse * mouse = Services::getRef()->getMouse();

    this->creatingMenu = &this->menu->createMenu()

    .setCoords(mouse->i_getMouseX(), mouse->i_getMouseY())
    .setDestroyCallback([](Menu * _self, void * _data) {

        ((UserControl *)_data)->creatingMenu = NULL;

    }, this)
    .addItemTitle("title", "... creating");

    if (this->state == UserControl::STATE_CIRCLE || this->state == UserControl::STATE_ELLIPSE) {
        // density control

        this->creatingMenu->addItem("inc_dens", "Increase density")
        .setLastItemCallback([](MenuItem * _self, void * _data) {

            ((UserControl *)_data)->drawHelp.addDensity(5);
            ((UserControl *)_data)->drawHelp.stackFinalPoints();

        }, this);

        this->creatingMenu->addItem("dec_dens", "Decrease density")
        .setLastItemCallback([](MenuItem * _self, void * _data) {

            ((UserControl *)_data)->drawHelp.addDensity(-5);
            ((UserControl *)_data)->drawHelp.stackFinalPoints();

        }, this);
    }

    if (this->state == UserControl::STATE_POLYLINE) {
        // polyline

        this->creatingMenu->addItem("close", "Finish closed")
        .setLastItemCallback([](MenuItem * _self, void * _data) {

            ((UserControl *)_data)->drawHelp.pushToDrawArea(((UserControl *)_data)->area, true);
            ((UserControl *)_data)->stateTransfer(UserControl::STATE_NONE);

        }, this);

        this->creatingMenu->addItem("close", "Finish")
        .setLastItemCallback([](MenuItem * _self, void * _data) {

            ((UserControl *)_data)->drawHelp.pushToDrawArea(((UserControl *)_data)->area, false);
            ((UserControl *)_data)->stateTransfer(UserControl::STATE_NONE);

        }, this);

    } else {
        this->creatingMenu->addItem("finish", "Finish")
        .setLastItemCallback([](MenuItem * _self, void * _data) {

            ((UserControl *)_data)->drawHelp.pushToDrawArea(((UserControl *)_data)->area);
            ((UserControl *)_data)->stateTransfer(UserControl::STATE_NONE);

        }, this);
    }

    this->creatingMenu->addItem("close", "Cancel")
    .setLastItemCallback([](MenuItem * _self, void * _data) {

        ((UserControl *)_data)->stateTransfer(UserControl::STATE_NONE);

    }, this)
    .position();
}

void UserControl::createModifyMenu(bool _keepCoord) {

    IMouse * mouse = Services::getRef()->getMouse();

    int coordX = mouse->i_getMouseX();
    int coordY = mouse->i_getMouseY();

    if (this->modifyMenu != NULL) {
        if (_keepCoord) {
            coordX = this->modifyMenu->getCoordX();
            coordY = this->modifyMenu->getCoordY();
        }

        this->modifyMenu->destroy();
        this->modifyMenu = NULL;
    }

    this->modifyMenu = &this->menu->createMenu()

    .setCoords(coordX, coordY)
    .setDestroyCallback([](Menu * _self, void * _data) {

        ((UserControl *)_data)->modifyMenu = NULL;

    }, this);

    std::stringstream str;

    if (this->manNode != this->area->getRootNode()) {
        str << this->manNode->getPathReadable() << " | " ;
    }

    str << this->manNode->getTypeReadable(false);

    this->modifyMenu->addItemTitle("1_caption", str.str().c_str());

    if (this->manNode->getType() == DrawNode::TYPE_GROUP) {
        // create entities under group

        this->modifyMenu->addItem("polyline", "Create line, polyline")
        .setLastItemCallback([](MenuItem * _self, void * _data) {

            ((UserControl *)_data)->stateTransfer(UserControl::STATE_POLYLINE);

        }, this);

        this->modifyMenu->addItem("square", "Create square")
        .setLastItemCallback([](MenuItem * _self, void * _data) {

            ((UserControl *)_data)->stateTransfer(UserControl::STATE_SQUARE);

        }, this);

        this->modifyMenu->addItem("rectangle", "Create rectangle")
        .setLastItemCallback([](MenuItem * _self, void * _data) {

            ((UserControl *)_data)->stateTransfer(UserControl::STATE_RECTANGLE);

        }, this);

        this->modifyMenu->addItem("circle", "Create circle")
        .setLastItemCallback([](MenuItem * _self, void * _data) {

            ((UserControl *)_data)->stateTransfer(UserControl::STATE_CIRCLE);

        }, this);

        this->modifyMenu->addItem("ellipse", "Create ellipse")
        .setLastItemCallback([](MenuItem * _self, void * _data) {

            ((UserControl *)_data)->stateTransfer(UserControl::STATE_ELLIPSE);

        }, this);

        this->modifyMenu->addItemTitle("2_caption", "... modify");

    } else {
        // fill style picking

        this->modifyMenu->addItem("3_fill", "Set fill style")
        .setLastItemCallback([](MenuItem * _self, void * _data) {

            _self->getCollector()->createMenu()
            .setParentItem(_self)
            .addItem("0_fill", "None")
            .setLastItemCallback([](MenuItem * _self, void * _data) {

                ( (DrawEntity *)((UserControl *)_data)->manNode )->getFiller()->setStyle(FillStyle::STYLE_NONE);

            }, _data)
            .addItem("1_fill", "Solid")
            .setLastItemCallback([](MenuItem * _self, void * _data) {

                ( (DrawEntity *)((UserControl *)_data)->manNode )->getFiller()->setStyle(FillStyle::STYLE_SOLID);

            }, _data)
            .addItem("2_fill", "Hatching A")
            .setLastItemCallback([](MenuItem * _self, void * _data) {

                ( (DrawEntity *)((UserControl *)_data)->manNode )->getFiller()->setStyle(FillStyle::STYLE_LINED45);

            }, _data)
            .addItem("3_fill", "Hatching B")
            .setLastItemCallback([](MenuItem * _self, void * _data) {

                ( (DrawEntity *)((UserControl *)_data)->manNode )->getFiller()->setStyle(FillStyle::STYLE_LINED45_REV);

            }, _data)
            .addItem("4_fill", "Squares")
            .setLastItemCallback([](MenuItem * _self, void * _data) {

                ( (DrawEntity *)((UserControl *)_data)->manNode )->getFiller()->setStyle(FillStyle::STYLE_LINECROSS);

            }, _data)
            .position();

        }, this);

        // fill color picking

        this->modifyMenu->addItem("4_color", "Set fill color")
        .setLastItemCallback([](MenuItem * _self, void * _data) {

            Menu * subMenu = &_self->getCollector()->createMenu();

            for (auto _it = Color::colors.begin(); _it != Color::colors.end(); ++_it) {
                subMenu->addItem((*_it).first.c_str(), (*_it).first.c_str())
                .setLastItemCallback([](MenuItem * _self, void * _data) {

                    ( (DrawEntity *)((UserControl *)_data)->manNode )->getFiller()->setColor(Color(_self->getKey().c_str()));

                }, _data);
            }

            subMenu->setParentItem(_self).position();

        }, this);
    }

    // move

    this->modifyMenu->addItem("move", "Move")
    .setLastItemCallback([](MenuItem * _self, void * _data) {

        ((UserControl *)_data)->stateTransfer(UserControl::STATE_MOVE);

    }, this);

    // link creation

    if (this->manNode->getType() == DrawNode::TYPE_GROUP && this->manNode->getParentNode() != NULL) {

        this->modifyMenu->addItem("link", "Create link")
        .setLastItemCallback([](MenuItem * _self, void * _data) {

            DrawNode * nd = ((UserControl *)_data)->manNode;
            DrawShortcut * shNd = new DrawShortcut(nd);

            shNd->getTranslate().addOffset(20, 20);
            shNd->getTranslate().applyFixed();

            nd->getParentNode()->addNode(shNd);

            shNd->setNodeKeyUnique(nd->getNodeKey().c_str());

            ((UserControl *)_data)->manNode = shNd;
            ((UserControl *)_data)->createModifyMenu(true);

        }, this);

    } else if (this->manNode->getType() == DrawNode::TYPE_SHORTCUT && this->manNode->getParentNode() != NULL) {
        // duplicate of that link

        this->modifyMenu->addItem("link", "Create link")
        .setLastItemCallback([](MenuItem * _self, void * _data) {

            DrawShortcut * shCurNd = (DrawShortcut *)( ((UserControl *)_data)->manNode );
            DrawNode * nd = shCurNd->getGroupLink();
            DrawShortcut * shNd = new DrawShortcut(nd);

            shNd->getTranslate().translate(shCurNd->getTranslate());
            shNd->getTranslate().addOffset(20, 20);
            shNd->getTranslate().applyFixed();

            nd->getParentNode()->addNode(shNd);

            shNd->setNodeKeyUnique(nd->getNodeKey().c_str());

            ((UserControl *)_data)->manNode = shNd;
            ((UserControl *)_data)->createModifyMenu(true);

        }, this);

    }

    // cloning

    if (
        this->manNode->getParentNode() != NULL &&

        (
            this->manNode->getType() == DrawNode::TYPE_GROUP ||
            this->manNode->getType() == DrawNode::TYPE_ENTITY ||
            this->manNode->getType() == DrawNode::TYPE_CIRCLE ||
            this->manNode->getType() == DrawNode::TYPE_ELLIPSE ||
            this->manNode->getType() == DrawNode::TYPE_SQUARE ||
            this->manNode->getType() == DrawNode::TYPE_RECTANGLE
        )
    ) {

        this->modifyMenu->addItem("duplicate", "Duplicate")
        .setLastItemCallback([](MenuItem * _self, void * _data) {

            DrawNode * nd = ((UserControl *)_data)->manNode;
            DrawNode * dupNd = nd->getParentNode()->addNodeDuplicate(nd);

            ((UserControl *)_data)->manNode = dupNd;
            ((UserControl *)_data)->createModifyMenu(true);

        }, this);
    }

    // delete

    if (this->manNode->getParentNode() != NULL) {

        this->modifyMenu->addItem("delete", "Delete")
        .setLastItemCallback([](MenuItem * _self, void * _data) {

            DrawNode * nd = ((UserControl *)_data)->manNode;

            nd->getParentNode()->delNode(nd);

            ((UserControl *)_data)->stateTransfer(UserControl::STATE_NONE);

        }, this);
    }

    // group list

    if (this->manNode->getParentNode() != NULL) {

        this->modifyMenu->addItem("groups", "... parent groups")
        .setLastItemCallback([](MenuItem * _self, void * _data) {

            Menu * grpMenu = &_self->getCollector()->createMenu().setParentItem(_self);
            DrawNode * loop = ((UserControl *)_data)->manNode->getParentNode();

            grpMenu->custData = _data;

            while (loop != NULL) {
                grpMenu->addItem(loop->getNodeKey().c_str(), loop->getNodeKey().c_str());
                grpMenu->setLastItemCallback([](MenuItem * _self, void * _data) {

                    ((UserControl *)_self->getParent()->custData)->manNode = (DrawNode *)_data;
                    ((UserControl *)_self->getParent()->custData)->createModifyMenu(true);

                }, loop);

                loop = loop->getParentNode();
            }

            grpMenu->position();

        }, this);
    }

    if (this->manNode->haveGrpChilds()) {

        this->modifyMenu->addItem("ch_groups", "... child groups")
        .setLastItemCallback([](MenuItem * _self, void * _data) {

            Menu * grpMenu = &_self->getCollector()->createMenu().setParentItem(_self);
            DrawNode * cNode = ((UserControl *)_data)->manNode;

            grpMenu->custData = _data;

            for (auto _it = cNode->getChilds().begin(); _it != cNode->getChilds().end(); ++_it) {

                DrawNode * loop = *_it;

                if (loop->getType() == DrawNode::TYPE_GROUP) {
                    // group focus

                    grpMenu->addItem(loop->getNodeKey().c_str(), loop->getNodeKey().c_str());
                    grpMenu->setLastItemCallback([](MenuItem * _self, void * _data) {

                        ((UserControl *)_self->getParent()->custData)->manNode = (DrawNode *)_data;
                        ((UserControl *)_self->getParent()->custData)->createModifyMenu(true);

                    }, loop);

                } else if (loop->getType() == DrawNode::TYPE_SHORTCUT) {
                    // shortcut focus

                    DrawShortcut * shNode = (DrawShortcut *)loop;

                    grpMenu->addItem("sh", std::string().append(shNode->getNodeKey()).append(" [sh] ").append(shNode->getGroupLink()->getNodeKey()).c_str());
                    grpMenu->setLastItemCallback([](MenuItem * _self, void * _data) {

                        ((UserControl *)_self->getParent()->custData)->manNode = (DrawNode *)_data;
                        ((UserControl *)_self->getParent()->custData)->createModifyMenu(true);

                    }, loop);

                }
            }

            grpMenu->position();

        }, this);
    }

    this->modifyMenu->addItem("close", "Cancel")
    .setLastItemCallback([](MenuItem * _self, void * _data) {

        ((UserControl *)_data)->stateTransfer(UserControl::STATE_NONE);

    }, this);

    this->modifyMenu->position();
}

void UserControl::stateTransfer(int _newState) {

    if (this->rootMenu != NULL) {
        this->rootMenu->destroy();
        this->rootMenu = NULL;
    }

    if (this->createStateMenu != NULL) {
        this->createStateMenu->destroy();
        this->createStateMenu = NULL;
    }

    if (this->creatingMenu != NULL) {
        this->creatingMenu->destroy();
        this->creatingMenu = NULL;
    }

    if (this->modifyMenu != NULL) {
        this->modifyMenu->destroy();
        this->modifyMenu = NULL;
    }

    switch (this->state) {
        case UserControl::STATE_POLYLINE :
        case UserControl::STATE_SQUARE :
        case UserControl::STATE_RECTANGLE :
        case UserControl::STATE_CIRCLE :
        case UserControl::STATE_ELLIPSE :
            this->drawHelp.resetClean();
            break;
    }

    std::stringstream str;

    this->state = _newState;

    switch (_newState) {
        case UserControl::STATE_POLYLINE :

            this->drawHelp.setState(this->state, this->manNode);

            this->createStateMenu = &this->menu->createMenu()

            .setCoords(0, 0)
            .addItemTitle("def", "Creating line, polyline")
            .setDestroyCallback([](Menu * _self, void * _data) {

                ((UserControl *)_data)->createStateMenu = NULL;

            }, this)
            .position();
            break;

        case UserControl::STATE_SQUARE :

            this->drawHelp.setState(this->state, this->manNode);

            this->createStateMenu = &this->menu->createMenu()

            .setCoords(0, 0)
            .addItemTitle("def", "Creating square")
            .setDestroyCallback([](Menu * _self, void * _data) {

                ((UserControl *)_data)->createStateMenu = NULL;

            }, this)
            .position();
            break;

        case UserControl::STATE_RECTANGLE :

            this->drawHelp.setState(this->state, this->manNode);

            this->createStateMenu = &this->menu->createMenu()

            .setCoords(0, 0)
            .addItemTitle("def", "Creating rectangle")
            .setDestroyCallback([](Menu * _self, void * _data) {

                ((UserControl *)_data)->createStateMenu = NULL;

            }, this)
            .position();
            break;

        case UserControl::STATE_CIRCLE :

            this->drawHelp.setState(this->state, this->manNode);

            this->createStateMenu = &this->menu->createMenu()

            .setCoords(0, 0)
            .addItemTitle("def", "Creating circle")
            .setDestroyCallback([](Menu * _self, void * _data) {

                ((UserControl *)_data)->createStateMenu = NULL;

            }, this)
            .position();
            break;

        case UserControl::STATE_ELLIPSE :

            this->drawHelp.setState(this->state, this->manNode);

            this->createStateMenu = &this->menu->createMenu()

            .setCoords(0, 0)
            .addItemTitle("def", "Creating ellipse")
            .setDestroyCallback([](Menu * _self, void * _data) {

                ((UserControl *)_data)->createStateMenu = NULL;

            }, this)
            .position();
            break;

        case UserControl::STATE_MODIFY :
            str << "Modify of " << this->manNode->getTypeReadable(true);

            this->createStateMenu = &this->menu->createMenu()

            .setCoords(0, 0)
            .addItemTitle("def", str.str().c_str())
            .setDestroyCallback([](Menu * _self, void * _data) {

                ((UserControl *)_data)->createStateMenu = NULL;

            }, this)
            .position();

            this->createModifyMenu(true);
            break;

        case UserControl::STATE_MOVE :
            str << "Moving of " << this->manNode->getTypeReadable(true);

            this->createStateMenu = &this->menu->createMenu()

            .setCoords(0, 0)
            .addItemTitle("def", str.str().c_str())
            .setDestroyCallback([](Menu * _self, void * _data) {

                ((UserControl *)_data)->createStateMenu = NULL;

            }, this)
            .position();
            break;
    }
}

void UserControl::draw(GraphicPrinter & print) {

    this->drawHelp.draw(print);

}
