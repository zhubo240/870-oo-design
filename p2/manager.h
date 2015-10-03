#include <SDL.h>
#include <vector>

#include "ioManager.h"
#include "clock.h"
#include "sprite.h"

using std::vector;

class Manager {
public:
	Manager();
	~Manager();
	void play();

private:
	const bool env;
	const IOManager* io;
	Clock& clock;

	SDL_Surface * const screen;
	int backRed;
	int backGreen;
	int backBlue;
	SDL_Surface* bgSurface;
	const Frame* const bgFrame;

	SDL_Surface* orbSurface;
	const Frame * const orbFrame;
	Sprite orb;
	vector<Sprite> orbs;

	bool makeVideo;
	int frameCount;
	std::string username;
	int frameMax;
	const std::string TITLE;

	void draw() const;
	void update();

	Manager(const Manager&);
	Manager& operator=(const Manager&);
	void drawBackground() const;
};
