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

class Clock;

struct SDL_Surface;

using std::vector;
using std::string;

class Hud {
public:
	Hud(const string& name);
	virtual ~Hud();

	void update(int ticks);

	void draw() const;
	void drawRect() const;
	void drawBord() const;
	void drawMsg() const;

	bool isVisiable;

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

	int isFirstAppear;
};

#endif /* HUD_H_ */
