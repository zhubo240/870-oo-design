#include "multisprite.h"

#include <cmath>
#include "sprite.h"
#include "gamedata.h"
#include "frameFactory.h"
#include "ExplodingMultiSprite.h"
#include "ioManager.h"

void MultiSprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
		currentFrame = (currentFrame + 1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

//TODO: why we nned to using this constructor to build a multiSprite for explosion
MultiSprite::MultiSprite(const std::string& name, const Vector2f& pos,
		const Vector2f& vel, vector<Frame*> frames) :
		Drawable(name, pos, vel,
				Gamedata::getInstance().getRandFloat(
						Gamedata::getInstance().getXmlFloat(name + "/zoom/min"),
						Gamedata::getInstance().getXmlFloat(
								name + "/zoom/max"))), explosion(NULL), mulframes(
				frames), worldWidth(
				Gamedata::getInstance().getXmlInt("world/width")), worldHeight(
				Gamedata::getInstance().getXmlInt("world/height")),

		currentFrame(0), numberOfFrames(
				Gamedata::getInstance().getXmlInt(name + "/frames")), frameInterval(
				Gamedata::getInstance().getXmlInt(name + "/frameInterval")), timeSinceLastFrame(
				0), frameWidth(mulframes[0]->getWidth()), frameHeight(
				mulframes[0]->getHeight()), strategies(), strategy() {
	this->initCollision();
}

MultiSprite::MultiSprite(const string& name) :
		Drawable(name,
				Vector2f(
						Gamedata::getInstance().getXmlInt(name + "/startLoc/x"),
						Gamedata::getInstance().getXmlInt(
								name + "/startLoc/y")),
				Vector2f(Gamedata::getInstance().getXmlInt(name + "/speedX"),
						Gamedata::getInstance().getXmlInt(name + "/speedY")),
				Gamedata::getInstance().getRandFloat(
						Gamedata::getInstance().getXmlFloat(name + "/zoom/min"),
						Gamedata::getInstance().getXmlFloat(
								name + "/zoom/max"))), explosion(NULL), mulframes(
				FrameFactory::getInstance().getFrames(name)), worldWidth(
				Gamedata::getInstance().getXmlInt("world/width")), worldHeight(
				Gamedata::getInstance().getXmlInt("world/height")),

		currentFrame(0), numberOfFrames(
				Gamedata::getInstance().getXmlInt(name + "/frames")), frameInterval(
				Gamedata::getInstance().getXmlInt(name + "/frameInterval")), timeSinceLastFrame(
				0), frameWidth(mulframes[0]->getWidth()), frameHeight(
				mulframes[0]->getHeight()), strategies(), strategy() {
	this->initCollision();
}

//TODO: do I need to write a copy constructor ? is there default one ?
MultiSprite::MultiSprite(const MultiSprite& s) :
		Drawable(s), explosion(s.explosion), mulframes(s.mulframes), worldWidth(
				s.worldWidth), worldHeight(s.worldHeight), currentFrame(
				s.currentFrame), numberOfFrames(numberOfFrames), frameInterval(
				frameInterval), timeSinceLastFrame(timeSinceLastFrame), frameWidth(
				frameWidth), frameHeight(frameHeight), strategies(), strategy() {
	this->initCollision();

}

MultiSprite::~MultiSprite(){
	delete this->explosion;

	for(unsigned i = 0; i < this->strategies.size(); i++)
		delete this->strategies[i];

}

void MultiSprite::draw() const {
	if (explosion) {
		explosion->draw();

		return;
	}

	Uint32 x = static_cast<Uint32>(X());
	Uint32 y = static_cast<Uint32>(Y());
	mulframes[currentFrame]->draw(x, y);
//	std::stringstream ss;
//	ss << "player postion : " << this->getPosition()[0] << ", "
//			<< this->getPosition()[1] << std::endl;
//	IOManager::getInstance().printMessageAt(ss.str(), 0, 80);
}

void MultiSprite::update(Uint32 ticks) {
	if (explosion) {
//		std::cout << "multisprite update" << std::endl;
		explosion->update(ticks);
		if (explosion->chunkCount() == 0) {
			delete explosion;
			explosion = NULL;
		}
		return;
	}

	advanceFrame(ticks);
	move(ticks);
}

bool MultiSprite::explode() {
	//std::cout << "player blow" << std::endl;

	if (explosion)
		return false;
	explosion = new ExplodingMultiSprite(*this);
	return true;
}

void MultiSprite::move(Uint32 ticks) {
	Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
	setPosition(getPosition() + incr);

	if (Y() < 0) {
		velocityY(abs(velocityY()));
	}
	if (Y() > worldHeight - frameHeight) {
		velocityY(-abs(velocityY()));
	}

	if (X() < 0) {
		velocityX(abs(velocityX()));
	}
	if (X() > worldWidth - frameWidth) {
		velocityX(-abs(velocityX()));
	}
}

int MultiSprite::getDistance(const Drawable* obj) const {
	return hypot(X() - obj->X(), Y() - obj->Y());
}


void MultiSprite::initCollision() {
		  this->strategies.push_back(new RectangularCollisionStrategy());
		  this->strategies.push_back(new MidPointCollisionStrategy());
		  this->strategies.push_back(new PerPixelCollisionStrategy());
		  this->strategy = this->strategies[2];
}

bool MultiSprite::collidedWith(const Drawable* d) const {
  return strategy->execute(*this, *d);
//	return true;
}
