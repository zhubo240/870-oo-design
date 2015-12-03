#ifndef FRAME__H
#define FRAME__H

#include <SDL/SDL_stdinc.h>
#include <SDL.h>
#include <string>

struct SDL_Surface;

class Frame {
public:
  Frame( const std::string&, SDL_Surface*);

  // ExplodingSprite::makeChunks needs this constructor if
  // we are not going to keep each chunk on a separate surface:
  Frame( SDL_Surface* spr, Uint16 w, Uint16 h, Sint16 src_x, Sint16 src_y);
  Frame(const Frame&);
  Frame& operator=(const Frame&);

  SDL_Surface* getSurface() const { return surface; }
  void draw(Sint16 x, Sint16 y) const;
  void draw(Sint16 x, Sint16 y, float angle, float scale, int dummy) const;
  void draw(Sint16 sx, Sint16 sy, Sint16 dx, Sint16 dy, float angle, float scale,
  		int dummy) const;

  // Chunk::draw needs the next function if the chunk is not
  // on a separate surface:
  void draw(Sint16 sx, Sint16 sy, Sint16 dx, Sint16 dy) const;
  void draw(Sint16 x, Sint16 y, double angle) const;

  Uint16 getWidth()  const { return width; }
  Uint16 getHeight() const { return height; }

  void setWidth(Uint16 w){
	  this->width = w;
  }
  void setHeight(Uint16 h){
	  this->height = h;
  }
  Uint16 getSourceX() const { return sourceX; }
  Uint16 getSourceY() const { return sourceY; }

private:
  SDL_Surface * screen;
  SDL_Surface * surface;
  Uint16 width;
  Uint16 height;
  Sint16 sourceX;
  Sint16 sourceY;

  Frame();
};

#endif
