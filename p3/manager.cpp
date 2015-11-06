#include "manager.h"

#include <SDL/SDL.h>
#include <SDL/SDL_events.h>
#include <SDL/SDL_keyboard.h>
#include <SDL/SDL_keysym.h>
#include <SDL/SDL_stdinc.h>
#include <SDL/SDL_video.h>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "drawable.h"
#include "gamedata.h"
#include "sprite.h"
#include "TwoWaySprite.h"
#include "vector2f.h"

Manager::~Manager() {
	std::list<Drawable*>::const_iterator ptr = sprites.begin();
	while (ptr != sprites.end()) {
		delete (*ptr);
		++ptr;
	}
}

Manager::Manager() :
		env(SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center"))), io(
				IOManager::getInstance()), clock(Clock::getInstance()), screen(
				io.getScreen()), worldFg("back",
				Gamedata::getInstance().getXmlInt("back/factor")),worldBg("back",
						Gamedata::getInstance().getXmlInt("back/factor")), viewport(
				Viewport::getInstance()),

		sprites(), currentSprite(),

		makeVideo(false), frameCount(0), username(
				Gamedata::getInstance().getXmlStr("username")), title(
				Gamedata::getInstance().getXmlStr("screenTitle")), frameMax(
				Gamedata::getInstance().getXmlInt("frameMax")) {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		throw string("Unable to initialize SDL: ");
	}
	SDL_WM_SetCaption(title.c_str(), NULL);
	atexit(SDL_Quit);

	//two way
	sprites.push_back(new TwoWaySprite("car"));

	//sprite
	sprites.push_back(new Sprite("ghost"));

	//sprite
	int n = Gamedata::getInstance().getXmlInt("triangle/count");
	MultiSprite tmp("triangle");
	Vector2f speed = tmp.getVelocity();
	double d = speed.magnitude();
	double l = 3.14 * d;
	double delta = l / n;
	for (int i = 0; i < n; i++) {
		MultiSprite* triangle = new MultiSprite("triangle");

		double seta = i * delta;
		double x = triangle->getVelocity().dot(Vector2f(cos(seta), -sin(seta)));
		double y = triangle->getVelocity().dot(Vector2f(sin(seta), cos(seta)));
		triangle->setVelocity(Vector2f(x, y));
		this->sprites.push_back(triangle);
	}

	currentSprite = sprites.begin();
	viewport.setObjectToTrack(*currentSprite);
}

void Manager::draw() const {
	worldFg.draw();
	clock.draw();
	std::list<Drawable*>::const_iterator ptr = sprites.begin();
	while (ptr != sprites.end()) {
		(*ptr)->draw();
		++ptr;
	}

	io.printMessageAt("Press T to switch sprites", 10, 70);
	io.printMessageAt(title, 10, 450);
	viewport.draw();

	SDL_Flip(screen);
}

// Move this to IOManager
void Manager::makeFrame() {
	std::stringstream strm;
	strm << "frames/" << username << '.' << std::setfill('0') << std::setw(4)
			<< frameCount++ << ".bmp";
	std::string filename(strm.str());
	std::cout << "Making frame: " << filename << std::endl;
	SDL_SaveBMP(screen, filename.c_str());
}

void Manager::switchSprite() {
	++currentSprite;
	if (currentSprite == sprites.end()) {
		currentSprite = sprites.begin();
	}
	viewport.setObjectToTrack(*currentSprite);
}

void Manager::update() {
	clock.update();
	Uint32 ticks = clock.getTicksSinceLastFrame();

	std::list<Drawable*>::const_iterator ptr = sprites.begin();
	while (ptr != sprites.end()) {
		(*ptr)->update(ticks);
		++ptr;
	}
	if (makeVideo && frameCount < frameMax) {
		makeFrame();
	}
	worldFg.update();
	viewport.update(); // always update viewport last
}

void Manager::play() {
	SDL_Event event;
	bool done = false;
	clock.start();

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
				if (keystate[SDLK_t]) {
					switchSprite();
				}
				if (keystate[SDLK_p]) {
					if (clock.isPaused())
						clock.unpause();
					else
						clock.pause();
				}
				if (keystate[SDLK_s]) {
					clock.toggleSloMo();
				}
				if (keystate[SDLK_F4] && !makeVideo) {
					std::cout << "Making video frames" << std::endl;
					makeVideo = true;
//				}if (keystate[SDLK_w]){
//					//update sprite the
//				}if (keystate[SDLK_s]){
//					//
//				}
				if (keystate[SDLK_a]){

				}if (keystate[SDLK_d]){

				}

			}
		}
		draw();
		update();
	}
}
