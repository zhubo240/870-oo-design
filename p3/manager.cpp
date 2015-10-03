#include "manager.h"

#include <SDL/SDL.h>
#include <SDL/SDL_events.h>
#include <SDL/SDL_keyboard.h>
#include <SDL/SDL_keysym.h>
#include <SDL/SDL_stdinc.h>
#include <SDL/SDL_video.h>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "gamedata.h"
#include "multisprite.h"
#include "sprite.h"

Manager::~Manager() {
	std::list<Drawable*>::const_iterator ptr = sprites.begin();
	while (ptr != sprites.end()) {
		delete (*ptr);
		++ptr;
	}
}

Manager::Manager() :
		env(SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center"))),
		io(IOManager::getInstance()), clock(Clock::getInstance()),
				screen(io.getScreen()),
				world("back", Gamedata::getInstance().getXmlInt("back/factor")),
				viewport(Viewport::getInstance()),

//		orbSurface(
//				io.loadAndSet(
//						Gamedata::getInstance().getXmlStr("greenorb/file"),
//						Gamedata::getInstance().getXmlBool(
//								"greenorb/transparency"))), orbFrame(
//				new Frame("greenorb", orbSurface)),
//
//		starSurface(
//				io.loadAndSet(
//						Gamedata::getInstance().getXmlStr("spinstar/file"),
//						Gamedata::getInstance().getXmlBool(
//								"spinstar/transparency"))), starFrames(), surfaces(),

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
/*
	SDL_Surface* surface = IOManager::getInstance().loadAndSet(
			Gamedata::getInstance().getXmlStr("spinstar/file"), true);
	unsigned numberOfFrames = Gamedata::getInstance().getXmlInt(
			"spinstar/frames");
	std::vector<SDL_Surface*> surfaces;
	starFrames.reserve(numberOfFrames);
	Uint16 srcX = Gamedata::getInstance().getXmlInt("spinstar/srcX");
	Uint16 srcY = Gamedata::getInstance().getXmlInt("spinstar/srcY");
	Uint16 width = Gamedata::getInstance().getXmlInt("spinstar/width");
	Uint16 height = Gamedata::getInstance().getXmlInt("spinstar/height");
	SDL_Surface* surf;
	for (unsigned i = 0; i < numberOfFrames; ++i) {
		unsigned frameX = i * width + srcX;
		surf = ExtractSurface::getInstance().get(surface, width, height, frameX,
				srcY);
		surfaces.push_back(surf);
		starFrames.push_back(new Frame("spinstar", surf));
	}
	SDL_FreeSurface(surface);
*/
	//TODO : need to change the constructor.
	//why using the container ?

	sprites.push_back(new MultiSprite("spinstar"));
	sprites.push_back(
			new Sprite("greenorb"));

	currentSprite = sprites.begin();
	viewport.setObjectToTrack(*currentSprite);
}

void Manager::draw() const {
	world.draw();
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
	world.update();
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
				}
			}
		}
		draw();
		update();
	}
}
