#ifndef DRAWABLE__H
#define DRAWABLE__H

#include <SDL/SDL_stdinc.h>
#include <SDL.h>
#include <string>

#include "vector2f.h"

#include "frame.h"

// Drawable is an Abstract Base Class (ABC) that
// specifies the methods that derived classes may
// and must have.
class Drawable {
public:
  Drawable(const std::string& n, const Vector2f& pos, const Vector2f& vel, float zoom) :
    name(n), position(pos), velocity(vel), zoom(zoom) {}

  Drawable(const Drawable& s) : 
    name(s.name), position(s.position), velocity(s.velocity), zoom(s.zoom)
    { }

  virtual ~Drawable() {}

  const std::string& getName() const { return name; }
  void setName(const std::string& n) { name = n;    }
  virtual const Frame* getFrame() const { std::cout << " no frame" << std::endl; return NULL;};

  virtual void draw() const = 0;
  virtual void update(Uint32 ticks) = 0;

  float X() const { return position[0]; }
  void X(float x) { position[0] = x;    }

  float Y() const { return position[1]; }
  void Y(float y) { position[1] = y;    }

  float velocityX() const  { return velocity[0]; }
  void velocityX(float vx) { velocity[0] = vx;   }
  float velocityY() const  { return velocity[1]; }
  void velocityY(float vy) { velocity[1] = vy;   }

const Vector2f& getVelocity() const   { return velocity; }
  void setVelocity(const Vector2f& vel) { velocity = vel;  }
  const Vector2f& getPosition() const   { return position; }
  void setPosition(const Vector2f& pos) { position = pos;  }

  virtual bool collidedWith(const Drawable*) const { 
    throw std::string("No collidedWith");  
  }

  virtual bool explode() { throw name+std::string(" can't explode");  }

  float getZoom() const { return this->zoom;}
  void setZoom(float zoom){this->zoom = zoom;}

private:
  std::string name;
  Vector2f position;
  Vector2f velocity;
  float zoom;
};
#endif
