#include <iostream>
#include <string>
#include <cmath>
#include <iomanip>
#include "sprite.h"
#include "gamedata.h"
#include "manager.h"

Manager::~Manager() {
	SDL_FreeSurface(orbSurface);
	delete orbFrame;
	SDL_FreeSurface(this->bgSurface);
	delete bgFrame;

	SDL_FreeSurface(this->screen);
}

Manager::Manager() :
		env(SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center"))), io(
				IOManager::getInstance()), clock(Clock::getInstance()), screen(
				io->getScreen()), backRed(
				Gamedata::getInstance()->getXmlInt("back/red")), backGreen(
				Gamedata::getInstance()->getXmlInt("back/green")), backBlue(
				Gamedata::getInstance()->getXmlInt("back/blue")), bgSurface(
				io->loadAndSet(Gamedata::getInstance()->getXmlStr("back/file"),
						false)), bgFrame(new Frame("back", bgSurface)),

		orbSurface(
				io->loadAndSet(
						Gamedata::getInstance()->getXmlStr("greenorb/file"),
						Gamedata::getInstance()->getXmlBool(
								"greenorb/transparency"))), orbFrame(
				new Frame("greenorb", orbSurface)), orb("greenorb", orbFrame),

		orbs(Gamedata::getInstance()->getXmlInt("greenorb/count"), orb),

		makeVideo(false), frameCount(0), username(
				Gamedata::getInstance()->getXmlStr("username")), frameMax(
				Gamedata::getInstance()->getXmlInt("frameMax")), TITLE(
				Gamedata::getInstance()->getXmlStr("screenTitle")) {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		throw string("Unable to initialize SDL: ");
	}
	atexit(SDL_Quit);

	//copy constructor
	Vector2f speed = this->orbs[0].getVelocity();
	double d = speed.magnitude();
	double l = 3.14 * d;
	double delta = l / this->orbs.size();
	for (unsigned int i = 0; i < this->orbs.size(); i++) {
		double seta = i * delta;
		double x = this->orbs[i].getVelocity().dot(
				Vector2f(cos(seta), -sin(seta)));
		double y = this->orbs[i].getVelocity().dot(
				Vector2f(sin(seta), cos(seta)));
		this->orbs[i].setVelocity(Vector2f(x, y));
	}
}

void Manager::drawBackground() const {
	this->bgFrame->draw(0, 0);
}

void Manager::draw() const {
	drawBackground();

	IOManager::getInstance()->printMessageCenteredAt(this->TITLE, 0);
	IOManager::getInstance()->printMessageAt(this->username,
			Gamedata::getInstance()->getXmlInt("world/width") - 100,
			Gamedata::getInstance()->getXmlInt("world/height") - 100);

	for (unsigned int i = 0; i < this->orbs.size(); i++)
		this->orbs[i].draw();

	clock.draw();
	SDL_Flip(screen);
}

void Manager::update() {
	clock.update();
	Uint32 ticks = clock.getTicksSinceLastFrame();
	for (unsigned int i = 0; i < this->orbs.size(); i++)
		this->orbs[i].update(ticks);

	if (makeVideo && frameCount < frameMax) {
		std::stringstream strm;
		strm << "frames/" << username << '.' << std::setfill('0')
				<< std::setw(4) << frameCount++ << ".bmp";
		std::string filename(strm.str());
		std::cout << "Making frame: " << filename << std::endl;
		SDL_SaveBMP(screen, filename.c_str());
	}
}

void Manager::play() {
	SDL_Event event;

	bool done = false;
	while (not done) {

		while (SDL_PollEvent(&event)) {
			Uint8 *keystate = SDL_GetKeyState(NULL);
			if (event.type == SDL_QUIT) {
				done = true;
				break;
			}
			if (event.type == SDL_KEYDOWN) {
				if (keystate[SDLK_ESCAPE] || keystate[SDLK_q]) {
					done = true;
					break;
				}

				if (keystate[SDLK_F4] && !makeVideo) {
					std::cout << "Making video frames" << std::endl;
					makeVideo = true;
				}
				if (keystate[SDLK_p]) {
					if (clock.isPaused())
						clock.unpause();
					else
						clock.pause();
				}
				if(keystate[SDLK_s]){
					clock.toggleSloMo();
				}
			}
		}
		draw();
		update();

	}
}
