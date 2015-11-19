#include "manager.h"

#include <SDL/SDL.h>
#include <SDL/SDL_events.h>
#include <SDL/SDL_keyboard.h>
#include <SDL/SDL_keysym.h>
#include <SDL/SDL_stdinc.h>
#include <SDL/SDL_video.h>
#include <algorithm>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "clock.h"
#include "ExplodingSprite.h"
#include "frame.h"
#include "gamedata.h"
#include "ioManager.h"
#include "multisprite.h"
#include "Player.h"
#include "vector2f.h"
#include "viewport.h"
#include "ViewRelatedSprite.h"

Manager::~Manager() {
	std::list<Drawable*>::const_iterator ptr = fgSprites.begin();
	while (ptr != fgSprites.end()) {
		delete (*ptr);
		++ptr;
	}

	for (std::list<Drawable*>::iterator iter = this->bgSprites.begin();
			iter != this->bgSprites.end(); iter++)
		delete (*iter);
}

Manager::Manager() :
		env(SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center"))), io(
				IOManager::getInstance()), clock(Clock::getInstance()), screen(
				io.getScreen()), nearBg("nearback"), farBg("farback"), viewport(
				Viewport::getInstance()),

		fgSprites(), bgSprites(),obs(), stars(),foodGroups(), currentSprite(),

		makeVideo(false), frameCount(0), username(
				Gamedata::getInstance().getXmlStr("username")), title(
				Gamedata::getInstance().getXmlStr("screenTitle")), frameMax(
				Gamedata::getInstance().getXmlInt("frameMax")), player(
				new Player("car")), hud(Hud("hud")), less() {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		throw string("Unable to initialize SDL: ");
	}
	SDL_WM_SetCaption(title.c_str(), NULL);
	atexit(SDL_Quit);

	init();
}

void Manager::init(){
	//player
	fgSprites.push_back(player);


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
		this->fgSprites.push_back(ob);
	}

	//generate stars
	n = Gamedata::getInstance().getXmlInt("star/count");
	for (int i = 0; i < n; i++) {
		int interval = Gamedata::getInstance().getXmlInt("world/width") / n;
		int height = this->screen->h;

		Drawable* star = new ViewRelatedSprite("star");
		star->setPosition(
				Vector2f(i * interval,
						(int) Gamedata::getInstance().getRandInRange(
								0.25 * height, 0.4 * height)));

		this->stars.push_back(star);
	}
	std::sort(stars.begin(), stars.end(), this->less);
	for (unsigned i = 0; i < stars.size(); i++)
		this->bgSprites.push_back(stars[i]);

	//generate food groups.
	n = Gamedata::getInstance().getXmlInt("food/group/count");

	for (int i = 0; i < n; i++) {
		int interval = Gamedata::getInstance().getXmlInt("world/width") / n;
		int height = this->screen->h;

		vector<Drawable*> foodGroup;
		int min = Gamedata::getInstance().getXmlInt("food/group/min");
		int max = Gamedata::getInstance().getXmlInt("food/group/max");
		for (int j = 0; j < Gamedata::getInstance().getRandInRange(min, max);
				j++) {
			//std::cout << "here" << std::endl;
			Drawable* food = new ExplodingSprite(Sprite("food"));
			food->setPosition(
					Vector2f(i * interval + j*Gamedata::getInstance().getXmlInt("food/width"),
							(int) Gamedata::getInstance().getRandInRange(
									0.3 * height, 0.8 * height)));
			foodGroup.push_back(food);
			this->fgSprites.push_back(food);
		}

		this->foodGroups.push_back(foodGroup);
	}

	currentSprite = fgSprites.begin();
	viewport.setObjectToTrack(*currentSprite);
}

void Manager::reset(){
	this->player = new Player("car");
	this->makeVideo = false;
	this->clock.reset();

	//TODO: some wrong happens.
	//this->~Manager();
	init();

}

void Manager::draw() const {
	farBg.draw();

//back
	{
		std::list<Drawable*>::const_iterator ptr = this->bgSprites.begin();
		while (ptr != bgSprites.end()) {
			(*ptr)->draw();
			++ptr;
		}
	}

	nearBg.draw();

//front
	{
		std::list<Drawable*>::const_iterator ptr = fgSprites.begin();
		while (ptr != fgSprites.end()) {
			(*ptr)->draw();
			++ptr;
		}
	}

	//io.printMessageAt("Press T to switch sprites", 10, 70);
	io.printMessageAt(title, 10, 450);
//	viewport.draw();
	if (hud.getVisiable()) {
		this->hud.draw();
	}

	SDL_Flip(screen);
}

bool Manager::checkCollision() {
	for (unsigned i = 0; i < this->obs.size(); i++) {
		int minx1 = obs[i]->X();
		int miny1 = obs[i]->Y();
		int maxx1 = minx1 + obs[i]->getFrame()->getWidth();
		int maxy1 = miny1 + obs[i]->getFrame()->getHeight();

		int minx2 = this->player->X();
		int miny2 = this->player->Y();
		int maxx2 = minx2 + player->getFrame()->getWidth();
		int maxy2 = miny2 + player->getFrame()->getHeight();

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
	if (currentSprite == fgSprites.end()) {
		currentSprite = fgSprites.begin();
	}
	viewport.setObjectToTrack(*currentSprite);
}

void Manager::update() {
	clock.update();
	Uint32 ticks = clock.getTicksSinceLastFrame();

	this->hud.update(ticks);

	std::list<Drawable*>::const_iterator ptr = fgSprites.begin();
	while (ptr != fgSprites.end()) {
		(*ptr)->update(ticks);
		++ptr;
	}

	{
		std::list<Drawable*>::const_iterator ptr = this->bgSprites.begin();
		while (ptr != bgSprites.end()) {
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
				if(keystate[SDLK_r]){
					reset();
					std::cout << "reset" << std::endl;
				}

				if (keystate[SDLK_F1]) {
					if (hud.getVisiable())
						hud.setVisiable(false);
					else
						hud.setVisiable(true);
				}

				if (keystate[SDLK_t]) {
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
				if (keystate[SDLK_w]) {
					player->up();

				}
				if (keystate[SDLK_s]) {
					player->down();
				}
				if (keystate[SDLK_a]) {
					player->left();
				}
				if (keystate[SDLK_d]) {
					player->right();
				}
			}

			if (event.type == SDL_KEYUP) {

				if (event.key.keysym.sym == SDLK_w) {
					this->player->stopY();
				}
				if (event.key.keysym.sym == SDLK_s) {
					this->player->stopY();
				}
				if (event.key.keysym.sym == SDLK_a) {
					this->player->stopX();
				}
				if (event.key.keysym.sym == SDLK_d) {
					this->player->stopX();
				}

			}
		}
		draw();
		//done = checkCollision();
		update();

	}
}
