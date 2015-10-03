#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
#include "clock.h"
#include "gamedata.h"


Clock& Clock::getInstance() {
	if (SDL_WasInit(SDL_INIT_VIDEO) == 0) {
		throw std::string("Must init SDL before Clock");
	}
	static Clock clock;
	return clock;
}

Clock::Clock() :
		ticks(0), started(false), paused(false), sloMo(false), sumOfTicks(0), lastPause(
				0), alreadyRun(0), allUpdates(0) {
	start();
}

Clock::Clock(const Clock& c) :
		ticks(c.ticks), started(c.started), paused(c.paused), sloMo(c.sloMo), sumOfTicks(
				c.sumOfTicks), lastPause(c.lastPause),alreadyRun(c.alreadyRun), allUpdates(
				c.allUpdates) {
	start();
}

void Clock::draw() const {
	int fps = this->getFps();
	string s("FPS = ");
	string suffix;
	for (int i = 0; fps > 0; i++) {
		suffix.push_back(fps % 10 + '0');
		fps = fps / 10;
	}
	for (int i = 0, j = suffix.size() - 1; i < j; i++, j--) {
		char t;
		t = suffix[i], suffix[i] = suffix[j], suffix[j] = t;
	}

	IOManager::getInstance()->printMessageAt(s + suffix, 30, 10);

	int allSecs;
	if(this->isPaused())
		allSecs = this->alreadyRun;
	else
		allSecs = (this->alreadyRun * 1000 + SDL_GetTicks() - this->lastPause)
			/ 1000;


	suffix.clear();
	s = string("time = ");

	for (int i = 0; allSecs > 0; i++) {
		suffix.push_back(allSecs % 10 + '0');
		allSecs = allSecs / 10;
	}
	for (int i = 0, j = suffix.size() - 1; i < j; i++, j--) {
		char t;
		t = suffix[i], suffix[i] = suffix[j], suffix[j] = t;
	}
	if(suffix.size() == 0) suffix = "0";
	IOManager::getInstance()->printMessageAt(s + suffix, 30, 40);
}
void Clock::update() {
	if (!this->isPaused()) {
		int tmp = SDL_GetTicks();
		int allRunTime = this->alreadyRun * 1000 + tmp
				- this->lastPause;
		double secs = (double) allRunTime / 1000;

		if (secs >= 0.001) {
			this->ticks = this->allUpdates / secs;
			if (this->sloMo)
				this->ticks /= 10;

		}
		this->sumOfTicks += this->ticks;

		this->allUpdates++;
	}
}

unsigned int Clock::getTicksSinceLastFrame() const {
	return ticks;
}

void Clock::toggleSloMo() {
	this->sloMo = !this->sloMo;
}

int Clock::getFps() const {
	if (this->isPaused()) {
		return this->allUpdates / this->alreadyRun;
	} else {
		if (sloMo)
			return this->ticks * 10;
		else
			return this->ticks;
	}
}

void Clock::start() {

	this->started = true;
	this->paused = false;
	this->lastPause = SDL_GetTicks();
}

void Clock::pause() {

	this->paused = true;
	this->started = false;
	this->ticks = 0;
	this->alreadyRun += (SDL_GetTicks() - this->lastPause) * 1. / 1000;
	this->lastPause = SDL_GetTicks();
}

void Clock::unpause() {

	this->started = true;
	this->paused = false;
	this->lastPause = SDL_GetTicks();
}

