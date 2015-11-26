/*
 * Hud.h
 *
 *  Created on: Nov 6, 2015
 *      Author: zhubo
 */

#ifndef HUD_H_
#define HUD_H_

#include <string>
#include <vector>
#include "BulletPool.h"

class Clock;

struct SDL_Surface;

using std::vector;
using std::string;

class Hud {
public:
	Hud(const string& name);
	Hud(const Hud& hud);
	Hud& operator=(const Hud& hud);
	virtual ~Hud();

	void update(int ticks);

	void draw() const;
	void drawRect() const;
	void drawBord() const;
	void drawMsg() const;
	void setVisiable(bool isVisiable);
	bool getVisiable() const;

private:
	SDL_Surface* screen;

	Clock& clock;

	int x;
	int y;
	int width;
	int height;

	int alpha;
	int thickness;
	std::vector<int> rectColors;
	std::vector<int> bordColors;
	int bordColor;


	int maxTime;
	int leftTime;
	bool isVisiable;
	int isFirstAppear;
};

#endif /* HUD_H_ */
