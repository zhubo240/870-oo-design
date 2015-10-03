#include "multisprite.h"

#include <SDL/SDL_stdinc.h>

#include "frame.h"
#include "frameFactory.h"
#include "gamedata.h"
#include "vector2f.h"


void MultiSprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

MultiSprite::MultiSprite( const std::string& name, 
                          const std::vector<Frame*>& f) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  frames( f ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),

  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval") ),
  timeSinceLastFrame( 0 ),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight())
{ }

MultiSprite::MultiSprite(const string& name) :
		Drawable(name,
		           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
		                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
		           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
		                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
		           ),
		  frames( FrameFactory::getInstance().getFrames(name) ),
		  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
		  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),

		  currentFrame(0),
		  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
		  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval") ),
		  timeSinceLastFrame( 0 ),
		  frameWidth(frames[0]->getWidth()),
		  frameHeight(frames[0]->getHeight())
{
}

//MultiSprite::MultiSprite(const MultiSprite& msp) {
//}

MultiSprite& MultiSprite::operator =(const MultiSprite&) {
}

void MultiSprite::draw() const { 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frames[currentFrame]->draw(x, y);
}

void MultiSprite::update(Uint32 ticks) { 
  advanceFrame(ticks);
}


