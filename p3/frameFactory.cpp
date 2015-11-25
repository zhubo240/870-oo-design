#include "frameFactory.h"

#include <SDL/SDL_stdinc.h>
#include <SDL/SDL_video.h>
#include <iostream>
#include <utility>

#include "extractSurface.h"
#include "ioManager.h"

FrameFactory::~FrameFactory() {
	for (std::map<string, SDL_Surface*>::iterator iter = this->surfaces.begin();
			iter != this->surfaces.end(); ++iter) {
		SDL_FreeSurface(iter->second);
	}

	for (std::map<string, std::vector<SDL_Surface*> >::iterator iter =
			this->multiSurfaces.begin(); iter != this->multiSurfaces.end();
			++iter) {
		for (unsigned j = 0; j < iter->second.size(); j++) {
			SDL_FreeSurface(iter->second[j]);
		}
	}

	for (std::map<string, Frame*>::iterator iter = this->frames.begin();
			iter != this->frames.end(); ++iter) {
		delete iter->second;
	}

	for (std::map<string, std::vector<Frame*> >::iterator iter =
			this->multiFrames.begin(); iter != this->multiFrames.end();
			++iter) {
		for (unsigned j = 0; j < iter->second.size(); j++) {
			delete iter->second[j];
		}
	}
}

FrameFactory& FrameFactory::getInstance() {
	static FrameFactory factory;
	return factory;
}

Frame* FrameFactory::getFrame(const std::string& name) {

	std::map<std::string, Frame*>::const_iterator pos = frames.find(name);
	if (pos == frames.end()) {
		SDL_Surface * const surface = IOManager::getInstance().loadAndSet(
				gdata.getXmlStr(name + "/file"),
				gdata.getXmlBool(name + "/transparency"));

		surfaces[name] = surface;
		Frame * const frame = new Frame(name, surface);
		frames[name] = frame;
		return frame;
	} else {
		return pos->second;
	}
}

std::vector<Frame*> FrameFactory::getFrames(const std::string& name) {
	// First search map to see if we've already made it:
	std::map<std::string, std::vector<Frame*> >::const_iterator pos =
			multiFrames.find(name);
	if (pos != multiFrames.end()) {
		return pos->second;
	}

	// It wasn't in the map, so we have to make the vector of Frames:
	SDL_Surface* surface = IOManager::getInstance().loadAndSet(
			gdata.getXmlStr(name + "/file"), gdata.getXmlBool(name + "/transparency"));
	unsigned numberOfFrames = gdata.getXmlInt(name + "/frames");
	std::vector<Frame*> frames;
	std::vector<SDL_Surface*> surfaces;
	frames.reserve(numberOfFrames);
	Uint16 srcX = gdata.getXmlInt(name + "/srcX");
	Uint16 srcY = gdata.getXmlInt(name + "/srcY");
	Uint16 width = gdata.getXmlInt(name + "/width");
	Uint16 height = gdata.getXmlInt(name + "/height");

	SDL_Surface* surf;
	for (unsigned i = 0; i < numberOfFrames; ++i) {
//		std::cout << i << std::endl;
		unsigned frameX = i * width + srcX;
		surf = ExtractSurface::getInstance().get(surface, width, height, frameX,
				srcY);
		surfaces.push_back(surf);
		frames.push_back(new Frame(name, surf));
	}
	SDL_FreeSurface(surface);
	multiSurfaces[name] = surfaces;
	multiFrames[name] = frames;
	return frames;
}
