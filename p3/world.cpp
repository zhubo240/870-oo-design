#include "world.h"

#include <SDL/SDL_video.h>
#include <iostream>

#include "frameFactory.h"
#include "gamedata.h"


World::World(const std::string& name, int fact) :
  io( IOManager::getInstance() ),
//  surface( io.loadAndSet(
//    Gamedata::getInstance().getXmlStr("back/file"),
//    Gamedata::getInstance().getXmlBool("back/transparency")) ),
  frame( FrameFactory::getInstance().getFrame(name) ),
  factor(fact),
    frameWidth( Gamedata::getInstance().getXmlInt(name + "/width") ),
	frameHeight(Gamedata::getInstance().getXmlInt(name + "/height")),
    viewX(0.0), viewY(0.0),
    view(Viewport::getInstance())
{}

void World::update() {
  viewX = static_cast<int>(view.X() / factor) % this->frameWidth;
  viewY = static_cast<int>(view.Y() / factor) % this->frameHeight ;

  //std::cout << this->surface->h << std::endl;
}

void World::draw() const {
//  frame->draw(viewX, viewY, 0, 0);

	//TODO : sdl draw .. do not need view info.

	frame->draw(viewX, viewY, 0, 0);
	frame->draw(0, viewY, frameWidth-viewX, 0);
	frame->draw(viewX, 0, 0, frameHeight - viewY);
	frame->draw(0, 0, frameWidth-viewX, frameHeight - viewY);

}

