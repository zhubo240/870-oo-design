#include <SDL.h>
#include <string>
#include <ctime>
#include <deque>
#include "ioManager.h"
class Manager;

class Clock {
public:
	static Clock& getInstance();  // This class is a Singleton
	unsigned int getTicksSinceLastFrame() const;
	unsigned int getTicksSinceInit() const {
		return sumOfTicks;
	}

	void toggleSloMo();
	bool isStarted() const {
		return started;
	}
	bool isPaused() const {
		return paused;
	}
	int getFps() const;

	void start();
	void pause();
	void unpause();
	void draw() const;  // For debugging
	void update();  // Increments time/ticks

private:
	unsigned int ticks;
	bool started;
	bool paused;
	bool sloMo;
	unsigned int sumOfTicks;
	//over and beign, last pause
	int lastPause;
	double alreadyRun; // secs
	unsigned int allUpdates;


	Clock();
	Clock(const Clock&);
	Clock&operator=(const Clock&);
};
