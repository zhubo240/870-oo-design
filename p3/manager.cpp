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
#include <algorithm>
#include <string>

#include "drawable.h"
#include "gamedata.h"
#include "sprite.h"
#include "TwoWaySprite.h"
#include "vector2f.h"
#include "clock.h"

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
				io.getScreen()), nearBg("nearback"), farBg("farback"), viewport(
				Viewport::getInstance()),

		sprites(), obs(), stars(), currentSprite(),

		makeVideo(false), frameCount(0), username(
				Gamedata::getInstance().getXmlStr("username")), title(
				Gamedata::getInstance().getXmlStr("screenTitle")), frameMax(
				Gamedata::getInstance().getXmlInt("frameMax")), runner(
				new TwoWaySprite("car")), isRunnerFly(false), hud(Hud("hud")){
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		throw string("Unable to initialize SDL: ");
	}
	SDL_WM_SetCaption(title.c_str(), NULL);
	atexit(SDL_Quit);

	//two way
	runner = new TwoWaySprite("car");
	sprites.push_back(runner);

	//generate obstacles
	int n = Gamedata::getInstance().getXmlInt("triangle/count");
	for (int i = 0; i < n; i++) {
		int interval = Gamedata::getInstance().getXmlInt("world/width") / n;
		int height = this->screen->h;

		Drawable* ob = new MultiSprite("triangle");
		ob->setPosition(
				Vector2f(i * interval,
						(int) Gamedata::getInstance().getRandInRange(0,
								height)));

		this->obs.push_back(ob);
		this->sprites.push_back(ob);
	}

	//generate stars
	n = Gamedata::getInstance().getXmlInt("star/count");
	for (int i = 0; i < n; i++) {
		int interval = Gamedata::getInstance().getXmlInt("world/width") / n;
		int height = this->screen->h;

		Drawable* star = new Sprite("star");
		star->setPosition(
				Vector2f(i * interval,
						(int) Gamedata::getInstance().getRandInRange(
								0.25 * height, 0.4 * height)));

		this->stars.push_back(star);
	}

	std::sort(stars.begin(), stars.end(), this->less);

	currentSprite = sprites.begin();
	viewport.setObjectToTrack(*currentSprite);
}

void Manager::draw() const {
	farBg.draw();
	//stars

	{
		std::vector<Drawable*>::const_iterator ptr = this->stars.begin();
		while (ptr != stars.end()) {
			(*ptr)->draw();
			++ptr;
		}
	}

	nearBg.draw();
	{
		std::list<Drawable*>::const_iterator ptr = sprites.begin();
		while (ptr != sprites.end()) {
			(*ptr)->draw();
			++ptr;
		}
	}

	io.printMessageAt("Press T to switch sprites", 10, 70);
	io.printMessageAt(title, 10, 450);
	viewport.draw();
	if (hud.isVisiable) {
		this->hud.draw();
	}

	//clock.draw();
	SDL_Flip(screen);
}

bool Manager::checkCollision() {
	for (int i = 0; i < this->obs.size(); i++) {
		int minx1 = obs[i]->X();
		int miny1 = obs[i]->Y();
		int maxx1 = minx1 + obs[i]->getFrame()->getWidth();
		int maxy1 = miny1 + obs[i]->getFrame()->getHeight();

		int minx2 = this->runner->X();
		int miny2 = this->runner->Y();
		int maxx2 = minx2 + runner->getFrame()->getWidth();
		int maxy2 = miny2 + runner->getFrame()->getHeight();

		//left up conner
		int minx = std::max(minx1, minx2);
		int miny = std::max(miny1, miny2);
		//right down conner
		int maxx = std::min(maxx1, maxx2);
		int maxy = std::min(maxy1, maxy2);

		if (!(minx > maxx || miny > maxy)) {
//			std::cout << "collision" << std::endl;
			return true;
		}

	}
	return false;
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

	this->hud.update(ticks);
	std::list<Drawable*>::const_iterator ptr = sprites.begin();
	while (ptr != sprites.end()) {
		(*ptr)->update(ticks);
		++ptr;
	}

	{
		std::vector<Drawable*>::const_iterator ptr = this->stars.begin();
		while (ptr != stars.end()) {
			(*ptr)->update(ticks);
			++ptr;
		}
	}

	if (makeVideo && frameCount < frameMax) {
		makeFrame();
	}
	farBg.update();
	nearBg.update();
	viewport.update(); // always update viewport last
}

void Manager::play() {
	SDL_Event event;
	bool done = false;
	clock.start();

	int i = 0;
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

				if (keystate[SDLK_F1]) {
					if (hud.isVisiable)
						hud.isVisiable = 0;
					else
						hud.isVisiable = 1;
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
				}
				if (keystate[SDLK_UP]) {
//					std::cout << "key down" << std::endl;
					if (!this->isRunnerFly) {
						int vx = this->runner->getVelocity()[0];
						runner->setVelocity(Vector2f(vx, -50));
						this->isRunnerFly = true;
					}
				}

			}

			if (event.type == SDL_KEYUP) {

//				if (keystate[SDLK_UP]) {
//					std::cout << "key up" << std::endl;
				if (this->isRunnerFly) {
					int vx = this->runner->getVelocity()[0];
					runner->setVelocity(Vector2f(vx, 50));
					this->isRunnerFly = false;
				}
//				}
			}
		}
		draw();
		//done = checkCollision();
		update();

	}
}
